/* 
 * File:   processing_task.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:00
 */

#include "ProcessingTask.h"

ProcessingTask::ProcessingTask() {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _thereIsMotion = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;
}

ProcessingTask::ProcessingTask(int capDeviceIndex, BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer) :
_capDeviceIndex(capDeviceIndex), _frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _thereIsMotion = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;
}

ProcessingTask::~ProcessingTask() {
}

void ProcessingTask::start() {
    MessageDealer::showMessage("Processing task started.");

    /* abre o dispositivo de vídeo*/
    _synchAndStatusDealer->setProcessingTaskErrorStatus(openAndConfigureVideoDevice());

    while (!_synchAndStatusDealer->getTasksErrorStatus()) {
        try {
            /* lê frame do dispositivo de vídeo */
            _cap.read(_rawFrame);

            /* lê 3 frame (anterior, atual, e próximo, todos serão iguais inicialmente) */

            /* adiciona o frame lido no buffer de frames */
            _frameBuffer->pushBackFrame(_rawFrame);

            /* redimensiona o frame lido (por questões de desempenho,
             * somente o frame redimensionado sera usado para processamento) */
            cv::resize(_rawFrame, _prevFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            /* converte para gray scale*/
            cvtColor(_prevFrame, _prevFrame, CV_BGR2GRAY);

            /* lê o segundo frame (frame corrente) */
            _cap.read(_rawFrame);
            _frameBuffer->pushBackFrame(_rawFrame);
            cv::resize(_rawFrame, _currentFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
            cvtColor(_currentFrame, _currentFrame, CV_BGR2GRAY);

            /* lê o terceiro frame (próximo frame) */
            _cap.read(_rawFrame);
            _frameBuffer->pushBackFrame(_rawFrame);
            cv::resize(_rawFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
            cvtColor(_nextFrame, _nextFrame, CV_BGR2GRAY);

            int cont = 0;
            /* fluxo normal de execução*/
            while (!_synchAndStatusDealer->getProcessingTaskErrorStatus() && cont < 400) {

                /* atualiza o frame anterior e o frame corrente */
                _prevFrame = _currentFrame;
                _currentFrame = _nextFrame;

                /* lê o próximo frame do dispositivo de vídeo */
                _cap.read(_rawFrame);

                /* adiciona o frame lido no buffer de frames */
                _frameBuffer->pushBackFrame(_rawFrame);

                /* atualiza o próximo frame usado para processamento (redimensiona e transforma em gray scale */
                cv::resize(_rawFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
                
                _result = _nextFrame;
                
                cvtColor(_nextFrame, _nextFrame, CV_BGR2GRAY);

                /* calcula a diferença entre os frames */
                cv::absdiff(_prevFrame, _nextFrame, _d1);
                cv::absdiff(_nextFrame, _currentFrame, _d2);

                /* faz o e lógico (AND) entre as duas diferenças calculadas e armazena em _motion */
                cv::bitwise_and(_d1, _d2, _motion);

                /* aplica threshold na imagem para que pequenas diferenças sejam ignoradas (diminuir erros e chiados) */
                cv::threshold(_motion, _motion, 35, 255, CV_THRESH_BINARY);

                /* aplica erosão para eliminar ruidos */
                cv::erode(_motion, _motion, _kernelErode);
                
                /* calcula a quantidade de movimento (número de mudanças de pixel */
                _numberOfChanges = detectMotion();

                // se aconteçeram muitas mudanças, assumimos que algo mudou na imagem
                if (_numberOfChanges >= _thereIsMotion) {
                    if (_numberOfSequence > 0) {
                        MessageDealer::showMessage("Existe evento ocorrendo");
                        //                        saveImg(result, DIR, EXT, DIR_FORMAT.c_str(), FILE_FORMAT.c_str());
                        //                        saveImg(result_cropped, DIR, EXT, DIR_FORMAT.c_str(), CROPPED_FILE_FORMAT.c_str());
                    }
                    _numberOfSequence++;
                } else {
                    _numberOfSequence = 0;
                    //cv::waitKey(DELAY);
                }

                cv::imshow("Motion", _motion);
                cv::imshow("Result", _result);
                if (cv::waitKey(30) >= 0) break;
                
                MessageDealer::showMessage("cont: " + std::to_string(cont));

                cont++;
            }
            _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
        } catch (std::exception & e) {
            _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
            MessageDealer::showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()));
        }
    }
    MessageDealer::showMessage("Processing thread finished execution!");
}

bool ProcessingTask::openAndConfigureVideoDevice() {
    bool statusError = false;

    try {
        // abre dispositivo de vídeo com o argumento passado (índice do dispositivo de vídeo)
        _cap.open(_capDeviceIndex);
        if (_cap.isOpened()) {
            // define o tamanho do frame (largura X altura)
            _cap.set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
            _cap.set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
        } else {
            _executionError = true;
            _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
            MessageDealer::showErrorMessage(std::string("Cannot open/configure the video device with index ") + std::to_string(_capDeviceIndex));
        }
    } catch (std::exception & e) {
        statusError = true;
        MessageDealer::showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()));
    }
    return statusError;
}

int ProcessingTask::detectMotion() {
    // calcula o desvio padrão (standard deviation)
    cv::Scalar mean, stddev;
    cv::meanStdDev(_motion, mean, stddev);

    // se não tem uma quantidade muito grande de mudanças, então a movimentação (motion) é real (evita falsos positivos por causa de ruidos)
    if (stddev[0] < _maxDeviation) {
        int number_of_changes = 0;
        int min_x = _motion.cols, max_x = 0;
        int min_y = _motion.rows, max_y = 0;
        // itera sobre a imagem e detecta mudanças (itera de 2 em dois por motivos de desempenho (metade do tempo que levaria)
        for (int j = 0; j < _motion.rows; j += 2) { // height (altura)
            for (int i = 0; i < _motion.cols; i += 2) { // width (largura)
                // checa se a intensidade é igual a 255 no pixel (j,i), se sim,
                // isso significa que o pixel é diferente na seguência de
                // imagens (_prevFrame, _currentFrame, _nextFrame)
                if (static_cast<int> (_motion.at<uchar>(j, i)) == 255) {
                    number_of_changes++;
                    if (min_x > i) min_x = i;
                    if (max_x < i) max_x = i;
                    if (min_y > j) min_y = j;
                    if (max_y < j) max_y = j;
                }
            }
        }
        if (number_of_changes) {
            //check if not out of bounds
            if (min_x - 10 > 0) min_x -= 10;
            if (min_y - 10 > 0) min_y -= 10;
            if (max_x + 10 < _result.cols - 1) max_x += 10;
            if (max_y + 10 < _result.rows - 1) max_y += 10;
            // draw rectangle round the changed pixel
            cv::Point x(min_x, min_y);
            cv::Point y(max_x, max_y);
            cv::Rect rect(x, y);
            cv::Mat cropped = _result(rect);
            cropped.copyTo(resultCropped);
            cv::rectangle(_result, rect, _color, 1);
        }
        return number_of_changes;
    }
    return 0;
}