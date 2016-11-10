/* 
 * File:   processing_task.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:00
 */

#include <core/mat.hpp>

#include "ProcessingTask.h"

ProcessingTask::ProcessingTask() {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _thereIsMotion = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfConsecutiveMotionSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;

    _synchAndStatusDealer->setMotionEventStatus(false);

    _servoHorizontalMovementEnable = false;
    _servoVerticalMovementEnable = false;

    _event = nullptr;
}

ProcessingTask::ProcessingTask(std::string events_storage_path, int capDeviceIndex, bool horizontal_tracking, bool vertical_tracking, std::list<Event*> * events_list,
        BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer, bool show_motion) :
_eventsStoragePath(events_storage_path), _capDeviceIndex(capDeviceIndex), _eventsList(events_list), _frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _thereIsMotion = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfConsecutiveMotionSequence = 0;
    _showMotion = _showMotion;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;

    _synchAndStatusDealer->setMotionEventStatus(false);

    _servoHorizontalMovementEnable = horizontal_tracking;
    _servoVerticalMovementEnable = vertical_tracking;

    _event = nullptr;
}

ProcessingTask::~ProcessingTask() {
}

void ProcessingTask::start() {
    MessageDealer::showMessage("Processing task started.");

    /* abre o dispositivo de vídeo*/
    _synchAndStatusDealer->setProcessingTaskErrorStatus(openAndConfigureVideoDevice());

    resetBackgroundModelFrames();

    /* estima o fps (taxa de frames por segundo */
    estimateFPS();

    try {
        /* lê 3 frame (anterior, atual, e próximo) para criar o background model */
        resetBackgroundModelFrames();

        /* fluxo normal de execução*/
        while (!_synchAndStatusDealer->ProcessingTaskHasError()) {

            /* atualiza o frame anterior e o frame corrente */
            _prevFrame = _currentFrame;
            _currentFrame = _nextFrame;

            /* lê o próximo frame do dispositivo de vídeo */
            _cap.read(_rawFrame);

            //            /* adiciona o frame lido no buffer de frames */
            //            _frameBuffer->pushBackFrame(_rawFrame);

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

            /* calcula a quantidade de movimento (número pixels que mudaram) */
            detectMotion();

            /* verifica se já existe um evento em aberto (evento occorendo) */
            if (_synchAndStatusDealer->getMotionEventStatus()) {
                manageEvent();
            } else {
                /* se aconteçeram muitas mudanças, assumimos que algo mudou na imagem */
                if (_thereIsValidMotion) {
                    if (_numberOfConsecutiveMotionSequence > 1) {
                        startEvent();
                        //manageEvent();
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

            if (cv::waitKey(1000/_fps) >= 0) break;
        }
        _cap.release();
        _synchAndStatusDealer->setProcessingTaskExecutionStatus(false);
        MessageDealer::showMessage("Processing thread exit while loop!");
    } catch (std::exception & e) {
        _cap.release();
        _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()));
    }

    MessageDealer::showMessage("Processing thread finished execution!");
}

std::string ProcessingTask::getFormatedTime(std::time_t raw_time, std::string format) {
    struct tm * timeinfo = localtime(&raw_time);
    char buffer [80];
    strftime(buffer, 80, format.c_str(), timeinfo);
    std::string str_time(buffer);

    return str_time;
}

void ProcessingTask::startEvent() {
    std::time_t time_now = std::time(nullptr);

    _synchAndStatusDealer->setMotionEventStatus(true);
    _eventStartTime = time_now;
    _lastMotionDetectedTime = time_now;
    _eventFramesCounter = 0;

    // o id do evento é a data que ele foi criado (garantindo assim um id único para cada evento criado)
    std::string event_id = getFormatedTime(time_now, "%Y-%m-%d-%H-%M-%S");
    std::string video_name = getFormatedTime(time_now, "%Y-%m-%d-%H-%M-%S");
    std::string video_extention = ".avi";

    _frameBuffer->pushBackFrame(_rawFrame, EVENT_START);

    _event = new Event(event_id, _eventsStoragePath, video_name, video_extention, _eventStartTime, _fps, cv::Size(_rawFrame.cols, _rawFrame.rows));
    MessageDealer::showMessage("Evento iniciado em " + getFormatedTime(time_now, "%H:%M:%S"));

    _lastMotionDetectedTime = time_now;
    followDetectedMotion();

    _eventFramesCounter++;
    if (_event) {
        _event->incrementFramesQuantity();
    }
}

void ProcessingTask::manageEvent() {
    std::time_t time_now = std::time(nullptr);

    if (_thereIsValidMotion) {
        _lastMotionDetectedTime = time_now;
        followDetectedMotion();

        _eventFramesCounter++;
        _frameBuffer->pushBackFrame(_rawFrame, INSIDE_EVENT);

        if (_event) {
            _event->incrementFramesQuantity();
        }
    } else {
        std::time_t event_duration = time_now - _eventStartTime;
        std::time_t time_without_motion = time_now - _lastMotionDetectedTime;

        bool no_motion_period = time_without_motion > EVENT_MAX_INACTIVITY_TIME;
        bool event_max_time_reached = event_duration > EVENT_MAX_DURATION;

        if (no_motion_period || event_max_time_reached) {
            if (_event) {
                double duration = difftime(time_now, _event->getStartTime());
                _event->setDuration(duration);
            }
            _frameBuffer->pushBackFrame(_rawFrame, EVENT_END);
            finalizeEvent();
            MessageDealer::showMessage("Evento finalizado em " + getFormatedTime(time_now, "%H:%M:%S"));
        } else {
            _eventFramesCounter++;
            _frameBuffer->pushBackFrame(_rawFrame, INSIDE_EVENT);
            if (_event) {
                _event->incrementFramesQuantity();
            }
        }
    }
}

void ProcessingTask::finalizeEvent() {
    if (_event) {
        //MessageDealer::showMessage("Dados do evento:" + _event->toString());

        _eventsList->push_front(_event);
        //delete _event;
        _event = nullptr;
    }

    std::list<Event*>::iterator it = _eventsList->begin();
    while (it != _eventsList->end()) {
        Event *my_event = dynamic_cast<Event*> (*it);
        _eventsList->erase(it++); // alternatively, i = items.erase(i);
        MessageDealer::showMessage("Dados do evento]:" + my_event->toString());
        delete my_event;
    }

    _synchAndStatusDealer->setMotionEventStatus(false);
}

void ProcessingTask::followDetectedMotion() {

    /* definição do tamanho do passo horizontal */
    if (_servoHorizontalMovementEnable) {
        short int horizontal_step = 0;

        if ((_motionCenter.x > FRAME_NEAR_CENTER_MAX_X) && (_motionCenter.x <= FRAME_NEAR_CENTER_MAX_X)) {
            short int horizontal_step = HORIZONTAL_NORMAL_STEP;
        } else if (_motionCenter.x > FRAME_NEAR_CENTER_MAX_X) {
            short int horizontal_step = HORIZONTAL_LONG_STEP;
        }

        /* movimentos horizontais */
        if (horizontal_step > 0) {
            if ((_quadrant == down_left || _quadrant == up_left) && (_horizontalDirection == left)) {
                servoHorizontalMovement(-horizontal_step); // valores negativos move para a esquerda
            }
            if ((_quadrant == down_right || _quadrant == up_right) && (_horizontalDirection == right)) {
                servoHorizontalMovement(+horizontal_step); // valores positivos move para a direita
            }
        }
    }

    /* definição do tamanho do passo vertical */
    if (_servoVerticalMovementEnable) {
        short int vertical_step = 0;

        if ((_motionCenter.y > FRAME_NEAR_CENTER_MAX_Y) && (_motionCenter.y <= FRAME_NEAR_CENTER_MAX_Y)) {
            short int vertical_step = HORIZONTAL_NORMAL_STEP;
        } else if (_motionCenter.y > FRAME_NEAR_CENTER_MAX_Y) {
            short int vertical_step = HORIZONTAL_LONG_STEP;
        }

        if (vertical_step > 0) {
            /* movimentos verticais */
            if ((_quadrant == down_left || _quadrant == down_right) && (_verticalDirection == down)) {
                servoVerticalMovement(-vertical_step); // valores positivos move para baixo
            }
            if ((_quadrant == up_left || _quadrant == up_right) && (_verticalDirection == up)) {
                servoVerticalMovement(+vertical_step); // valores positivos move para cima
            }
        }
    }
}

void ProcessingTask::servoHorizontalMovement(short int value) {
    std::stringstream ss;
    std::ofstream servo(SERVO_FILE);
    std::string write;
    short int position = _servoHorizontalPosition + value;

    if (position > SERVO_MAX_POSITION) {
        _servoHorizontalPosition = SERVO_MAX_POSITION;
    } else if (position < SERVO_MIN_POSITION) {
        _servoHorizontalPosition = SERVO_MIN_POSITION;
    } else {
        _servoHorizontalPosition = position;
    }

    ss.str("");
    ss << SERVOBLASTER_HORIZONTAL_MOVEMENT_CHANNEL_INDEX << "=" << _servoHorizontalPosition << "\n";
    write = ss.str();

    servo << write;
    servo.close();
    //cv::waitKey(DELAY);
}

void ProcessingTask::servoVerticalMovement(short int value) {
    std::stringstream ss;
    std::ofstream servo(SERVO_FILE);
    std::string write;
    short int position = _servoVerticalPosition + value;

    if (position > SERVO_MAX_POSITION) {
        _servoVerticalPosition = SERVO_MAX_POSITION;
    } else if (position < SERVO_MIN_POSITION) {
        _servoVerticalPosition = SERVO_MIN_POSITION;
    } else {
        _servoVerticalPosition = position;
    }

    ss.str("");
    ss << SERVOBLASTER_VERTICAL_MOVEMENT_CHANNEL_INDEX << "=" << _servoVerticalPosition << "\n";
    write = ss.str();

    servo << write;
    servo.close();
    //cv::waitKey(DELAY);
}

void ProcessingTask::resetBackgroundModelFrames() {
    /* reseta os centros de movimento para o meio do frame */
    _previousMotionCenter = DEFAULT_PROCESSED_FRAME_CENTER;
    _motionCenter = DEFAULT_PROCESSED_FRAME_CENTER;

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
    cv::Scalar mean, stddev; // mean: variancia, stddev: desvio padrão, cv::Scalar: 4-element vector derived from Vec

    /* 
     * The stddev tells us something about the distribution of motion.
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

    /*
     * C++: void meanStdDev(InputArray src, OutputArray mean, OutputArray stddev, InputArray mask=noArray())
     * Parameters:
     * src – input array that should have from 1 to 4 channels so that the results can be stored in Scalar_ ‘s.
     * mean – output parameter: calculated mean value.
     * stddev – output parameter: calculateded standard deviation.
     * mask – optional operation mask.
     */
    cv::meanStdDev(_motion, mean, stddev); // calcula a variância e desvio padrão de uma imagem

    /* se não tem uma quantidade muito grande de mudanças (menor que _maxDeviation),
     * então a movimentação (motion) é real (evita falsos positivos por causa de ruidos) */
    if (stddev[0] < _maxDeviation) {
        _numberOfChanges = 0;
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
                    _numberOfChanges++;
                    if (_motion_min_x > i) _motion_min_x = i;
                    if (_motion_max_x < i) _motion_max_x = i;
                    if (_motion_min_y > j) _motion_min_y = j;
                    if (_motion_max_y < j) _motion_max_y = j;
                }
            }
        }

        if (_numberOfChanges >= _thereIsMotion) {
            _thereIsValidMotion = true;
            if (_event) {
                _event->incrementMotionQuantity(_numberOfChanges);
            }

        } else {
            _thereIsValidMotion = false;
        }

        if (_thereIsValidMotion) { // number_of_changes > 0 significa true
            /* 
            //check if not out of bounds
            //            if (_motion_min_x - 10 > 0) _motion_min_x -= 10;
            //            if (_motion_min_y - 10 > 0) _motion_min_y -= 10;
            //            if (_motion_max_x + 10 < _result.cols - 1) _motion_max_x += 10;
            //            if (_motion_max_y + 10 < _result.rows - 1) _motion_max_y += 10;
             */

            if (_showMotion) {
                // draw rectangle round the changed pixel
                cv::Point x(_motion_min_x, _motion_min_y);
                cv::Point y(_motion_max_x, _motion_max_y);
                cv::Rect rect(x, y);

                /* desenha um retangulo no contorno da area onde ocorreu movimento */
                cv::rectangle(_result, rect, _color, 1);
            }

            _previousMotionCenter = _motionCenter;
            _motionCenter = cv::Point((_motion_min_x + _motion_max_x / 2), (_motion_min_y + _motion_max_y / 2));

            defineMotionDirection();
            defineMotionQuadrant();
            /*
            //            // recorta a imagem para apenas à area com movimento
            //            cv::Mat cropped = _result(rect);
            //            cropped.copyTo(resultCropped);
             */
        }
    }
}

void ProcessingTask::defineMotionDirection() {
    if (_previousMotionCenter.x < _motionCenter.x) {
        _horizontalDirection = right;
        if (_event) {
            _event->incrementHorizontalDirection(1);
        }
    } else {
        _horizontalDirection = left;
        if (_event) {
            _event->incrementHorizontalDirection(-1);
        }
    }

    if (_previousMotionCenter.y < _motionCenter.y) {
        _verticalDirection = up;
        if (_event) {
            _event->incrementVerticalDirection(1);
        }
    } else {
        _verticalDirection = down;
        if (_event) {
            _event->incrementVerticalDirection(-1);
        }
    }
}

void ProcessingTask::defineMotionQuadrant() {
    if (_motionCenter.x < DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y < DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = down_left;
    if (_motionCenter.x >= DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y < DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = down_right;
    if (_motionCenter.x >= DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y >= DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = up_right;
    if (_motionCenter.x < DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y >= DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = up_left;
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

void ProcessingTask::estimateFPS() {
    try {
        int num_frames = 120; // número de frames para capturar no treino
        int cont = 0;
        time_t start, end; // tempo inicial e final

        time(&start); // define o tempo inicial

        /* fluxo normal de execução*/
        while (!_synchAndStatusDealer->ProcessingTaskHasError() && cont < num_frames) {

            _prevFrame = _currentFrame;
            _currentFrame = _nextFrame;

            _cap.read(_rawFrame);

            cv::resize(_rawFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            _result = _nextFrame;

            cvtColor(_nextFrame, _nextFrame, CV_BGR2GRAY);

            cv::absdiff(_prevFrame, _nextFrame, _d1);
            cv::absdiff(_nextFrame, _currentFrame, _d2);

            cv::bitwise_and(_d1, _d2, _motion);

            cv::threshold(_motion, _motion, 35, 255, CV_THRESH_BINARY);

            cv::erode(_motion, _motion, _kernelErode);

            detectMotion();

            cont++;

            /* mostra os resultados da detecção de movimentos */
            cv::imshow("Motion", _motion);
            cv::imshow("Result", _result);

           if (cv::waitKey(30) >= 0) break;
        }

        time(&end); // define o tempo final

        double seconds = difftime(end, start); // tempo decorrido

        _fps = num_frames / seconds; // calcula a taxa de frames por segundo (aproximada)

        MessageDealer::showMessage("FPS estimado:" + std::to_string(_fps));
    } catch (std::exception & e) {
        _cap.release();
        _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in processing task while treining FPS rate: ") + std::string(e.what()));
    }
}