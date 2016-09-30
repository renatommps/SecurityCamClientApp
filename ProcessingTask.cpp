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
    _numberOfConsecutiveMotionSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;

    _thereIsEvent = false;
}

ProcessingTask::ProcessingTask(int capDeviceIndex, BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer) :
_capDeviceIndex(capDeviceIndex), _frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _thereIsMotion = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfConsecutiveMotionSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;

    _thereIsEvent = false;
}

ProcessingTask::~ProcessingTask() {
}

void ProcessingTask::start() {
    MessageDealer::showMessage("Processing task started.");

    /* abre o dispositivo de vídeo*/
    _synchAndStatusDealer->setProcessingTaskErrorStatus(openAndConfigureVideoDevice());

    try {
        /* lê 3 frame (anterior, atual, e próximo) para criar o background model */
        resetBackgroundModelFrames();

        int cont = 0;
        /* fluxo normal de execução*/
        while (!_synchAndStatusDealer->ProcessingTaskHasError() && cont < 400) {

            /* atualiza o frame anterior e o frame corrente */
            _prevFrame = _currentFrame;
            _currentFrame = _nextFrame;

            /* lê o próximo frame do dispositivo de vídeo */
            _cap.read(_rawFrame);

            /* adiciona o frame lido no buffer de frames */
            _frameBuffer->pushBackFrame(_rawFrame);

            /* atualiza o próximo frame usado para processamento (redimensiona e transforma em gray scale */
            cv::resize(_rawFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            /* frame resultante, que sera usado para mostrar o processamento */
            _result = _nextFrame;

            cvtColor(_nextFrame, _nextFrame, CV_BGR2GRAY);

            /* calcula a diferença entre os frames (Differential Substracting) */
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

            /* verifica se já existe um evento em aberto (evento occorendo) */
            if (_thereIsEvent) {
                manageEvent();
            } else {
                /* se aconteçeram muitas mudanças, assumimos que algo mudou na imagem */
                if (_thereIsValidMotion) {
                    if (_numberOfConsecutiveMotionSequence > 1) {
                        startEvent();
                        manageEvent();
                        //                        saveImg(result, DIR, EXT, DIR_FORMAT.c_str(), FILE_FORMAT.c_str());
                        //                        saveImg(result_cropped, DIR, EXT, DIR_FORMAT.c_str(), CROPPED_FILE_FORMAT.c_str());
                    }
                    _numberOfConsecutiveMotionSequence++;
                } else {
                    _numberOfConsecutiveMotionSequence = 0;
                }
            }

            /* mostra os resultados da detecção de movimentos */
            cv::imshow("Motion", _motion);
            cv::imshow("Result", _result);
            if (cv::waitKey(30) >= 0) break;

            MessageDealer::showMessage("cont: " + std::to_string(cont));

            cont++;
        }
        MessageDealer::showMessage("Processing thread exit while loop!");
        _synchAndStatusDealer->setProcessingTaskExecutionStatus(false);
    } catch (std::exception & e) {
        _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()));
    }

    MessageDealer::showMessage("Processing thread finished execution!");
}

void ProcessingTask::startEvent() {
    std::time_t time_now;
    time(&time_now); // seta a variável com o horário atual

    _thereIsEvent = true;
    _eventStartTime = time_now;
    _lastMotionDetectedTime = time_now;
    _eventFramesCounter = 0;

    MessageDealer::showMessage("Evento iniciado em " + std::string(time_now));
}

void ProcessingTask::manageEvent() {
    std::time_t time_now;
    time(&time_now); // seta a variável com o horário atual

    if (_thereIsValidMotion) {
        _lastMotionDetectedTime = time_now;
        followDetectedMotion();
    } else {
        std::time_t event_duration = time_now - _eventStartTime;
        std::time_t time_without_motion = time_now - _lastMotionDetectedTime;

        bool no_motion = time_without_motion > EVENT_MAX_INACTIVITY_TIME;
        bool event_max_time_reached = event_duration > EVENT_MAX_DURATION;

        if (no_motion || event_max_time_reached) {
            _thereIsEvent = false;
            MessageDealer::showMessage("Evento finalizado em " + std::string(time_now));
        } else {
            _eventFramesCounter++;
        }
    }
}

void ProcessingTask::followDetectedMotion() {
    /* movimentos horizontais */
    if ((_quadrant == down_left || _quadrant == up_left) && (_horizontalDirection == left)) {
        servoMoveLeft();
    }
    if ((_quadrant == down_right || _quadrant == up_right) && (_horizontalDirection == right)) {
        servoMoveRight();
    }

    /* movimentos verticais */
    if ((_quadrant == down_left || _quadrant == down_right) && (_verticalDirection == down)) {
        servoMoveDown();
    }
    if ((_quadrant == up_left || _quadrant == up_right) && (_verticalDirection == up)) {
        servoMoveUp()
    }

}

void ProcessingTask::resetBackgroundModelFrames() {

    /* reseta os centros de movimento para o meio do frame */
    _previousMotionCenter = FRAME_CENTER;
    _motionCenter = FRAME_CENTER;

    /* lê o primeiro frame (frame anterior) */
    _cap.read(_rawFrame);
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
}

void ProcessingTask::detectMotion() {

    cv::Scalar mean, stddev; // variancia e desvio padrão

    /* The stddev tells us something about the distribution of motion.
     * When motion is specific at a single point let's say a human which is
     * relative small against the size of the viewport (far away from the camera)
     * then the motion will be concentrated in a single point/pixel, in this case
     * the stddev will be near to zero.
     * When a lot of motion is detected and is distributed over the entire image,
     * than the stddev will be very high. A huge distribution mostly indicates no
     * real motion eg. indicate aggressive snow or other very abrupt changes.
     * Notice that in some scenarios, eg. public places, high distributions are
     * normal and this assumption fails.
     */
    cv::meanStdDev(_motion, mean, stddev); // calcula a variância e desvio padrão de uma imagem

    /* se não tem uma quantidade muito grande de mudanças (menor que _maxDeviation),
     * então a movimentação (motion) é real (evita falsos positivos por causa de ruidos) */
    if (stddev[0] < _maxDeviation) {
        int number_of_changes = 0;
        _motion_min_x = _motion.cols;
        _motion_min_y = _motion.rows;
        _motion_max_x = 0;
        _motion_max_y = 0;

        /* itera sobre a imagem e detecta mudanças (itera de 2 em dois por
         * motivos de desempenho, metade do tempo que levaria) */
        for (int j = 0; j < _motion.rows; j += 2) { // height (altura)
            for (int i = 0; i < _motion.cols; i += 2) { // width (largura)
                /* checa se a intensidade é igual a 255 no pixel (j,i),
                 * se sim, isso significa que o pixel é diferente na seguência
                 * de imagens (_prevFrame, _currentFrame, _nextFrame) */
                if (static_cast<int> (_motion.at<uchar>(j, i)) == 255) {
                    number_of_changes++;
                    if (_motion_min_x > i) _motion_min_x = i;
                    if (_motion_max_x < i) _motion_max_x = i;
                    if (_motion_min_y > j) _motion_min_y = j;
                    if (_motion_max_y < j) _motion_max_y = j;
                }
            }
        }

        if (_numberOfChanges >= _thereIsMotion) {
            _thereIsValidMotion = true;
        } else {
            _thereIsValidMotion = false;
        }

        if (_thereIsValidMotion) { // number_of_changes > 0 significa true
            //check if not out of bounds
            //            if (_motion_min_x - 10 > 0) _motion_min_x -= 10;
            //            if (_motion_min_y - 10 > 0) _motion_min_y -= 10;
            //            if (_motion_max_x + 10 < _result.cols - 1) _motion_max_x += 10;
            //            if (_motion_max_y + 10 < _result.rows - 1) _motion_max_y += 10;
            // draw rectangle round the changed pixel
            cv::Point x(_motion_min_x, _motion_min_y);
            cv::Point y(_motion_max_x, _motion_max_y);
            cv::Rect rect(x, y);

            _previousMotionCenter = _motionCenter;
            _motionCenter = cv::Point((_motion_min_x + _motion_max_x / 2), (_motion_min_y + _motion_max_y / 2));

            defineMotionDirection();
            defineMotionQuadrant();

            //            /* recorta a imagem para apenas à area com movimento */
            //            cv::Mat cropped = _result(rect);
            //            cropped.copyTo(resultCropped);

            /* desenha um retangulo no contorno da area onde ocorreu movimento */
            cv::rectangle(_result, rect, _color, 1);
        }
    }
    return 0;
}

void ProcessingTask::defineMotionDirection() {
    if (_previousMotionCenter.x < _motionCenter.x) {
        _horizontalDirection = right;
    } else {
        _horizontalDirection = left;
    }

    if (_previousMotionCenter.y < _motionCenter.y) {
        _verticalDirection = up;
    } else {
        _verticalDirection = down;
    }
}

void ProcessingTask::defineMotionQuadrant() {
    if (_motionCenter.x < FRAME_CENTER.x && _motionCenter.y < FRAME_CENTER.y) _quadrant = down_left;
    if (_motionCenter.x >= FRAME_CENTER.x && _motionCenter.y < FRAME_CENTER.y) _quadrant = down_right;
    if (_motionCenter.x >= FRAME_CENTER.x && _motionCenter.y >= FRAME_CENTER.y) _quadrant = up_right;
    if (_motionCenter.x < FRAME_CENTER.x && _motionCenter.y >= FRAME_CENTER.y) _quadrant = up_left;
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