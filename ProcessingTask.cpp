#include <core/mat.hpp>
#include <thread>
#include "ProcessingTask.h"

ProcessingTask::ProcessingTask() {
    _kernelErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    _minMotionValue = 5;
    _maxDeviation = 20;
    _numberOfChanges = 0;
    _numberOfConsecutiveMotionSequence = 0;

    cv::Scalar myColor(0, 255, 255);
    _color = myColor;

    _servoHorizontalMovementEnable = false;
    _servoVerticalMovementEnable = false;

    _event_running = false;
    _executionError = false;

    _showMotion = true;
}

ProcessingTask::~ProcessingTask() {
}

void ProcessingTask::start() {
    MessageDealer::showMessage("Processing task started.");

    /* abre o dispositivo de vídeo */
    openAndConfigureVideoDevice();

    /* lê 3 frame (anterior, atual, e próximo) para criar o modelo de fundo
     * (background model) para diferenciar o que é fundo, e o que é movimento
     */
    resetBackgroundModelFrames();

    /* estima o fps (taxa de quadros por segundo) */
    estimateFPS();

    try {
        /* lê 3 frame (anterior, atual, e próximo) para criar o background model */
        resetBackgroundModelFrames();

        /* fluxo normal de execução*/
        while (!_executionError) {

            /* atualiza o frame anterior e o frame corrente */
            _prevFrame = _currentFrame;
            _currentFrame = _nextFrame;

            /* lê o próximo frame do dispositivo de vídeo */
            _cap.read(_originalFrame);

            if (_event_running) {
                /* adiciona o frame original no buffer de frames */
                event_task->addFrameToBuffer(Frame(_streamingFrame, std::time(nullptr)));
            } else {
                /* adiciona o frame original no buffer de frames */
                _frameBuffer->push_back(Frame(_streamingFrame, std::time(nullptr)));
                if(_frameBuffer->size() > MAX_FRAME_BUFFER_SIZE){
                    _frameBuffer->pop_front();
                }
            }
            
            /* atualiza o Frame de streaming (mudar depois, esta ERRADO, tem de ser em outra thread !!!!!!!!!!!!!) */
            _streamingFrame = Frame(_streamingFrame, std::time(nullptr));

            /* redimensiona o próximo frame usado para processamento */
            cv::resize(_originalFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            /* atualiza frame resultante, que sera usado para mostrar o processamento (temporário, para testes, remover no futuro) */
            _result = _nextFrame;

            /* transforma em gray scale o próximo frame usado para processamento */
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
            if (_event_running) {
                /* gerencia um evento já criado */
                manageEvent();
            } else {
                /* se aconteçeram muitas mudanças, assumimos que algo mudou na imagem */
                if (_thereIsValidMotion) {
                    if (_numberOfConsecutiveMotionSequence > 1) {
                        /* inicia um novo evento */
                        startEvent();
                        // saveImg(result, DIR, EXT, DIR_FORMAT.c_str(), FILE_FORMAT.c_str());
                        // saveImg(result_cropped, DIR, EXT, DIR_FORMAT.c_str(), CROPPED_FILE_FORMAT.c_str());
                    }
                    _numberOfConsecutiveMotionSequence++;
                } else {
                    /* zera o número de frames consecutivos com movimento válido
                     * (válido para que seja considerável como um evento) */
                    _numberOfConsecutiveMotionSequence = 0;
                }
            }

            /* se não existe evento, e o buffer de frames é maior que o tamanho
             * máximo permitido, então remove o primeiro elemento */
            if (!_event_running && _frameBuffer->size() > 10) {
                _frameBuffer->pop_front();
            }

            /* mostra os resultados da detecção de movimentos */
            cv::imshow("Motion", _motion);
            cv::imshow("Result", _result);

            if (cv::waitKey(1000 / _fps) >= 0) break;
        }
        _cap.release();

        if (_event_running) {
            finalizeEvent();
        }
        MessageDealer::showMessage("Processing thread exit while loop!");
    } catch (std::exception & e) {
        _cap.release();

        if (_event_running) {
            finalizeEvent();
        }
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

    _eventStartTime = time_now;
    _lastMotionDetectedTime = time_now;

    _eventMotionQuantity = 0;
    _eventHorizontalDirection = 0;
    _eventVerticalDirection = 0;

    followDetectedMotion();

    event_task = new Event(_frameBuffer);
    std::thread event_thread(&Event::start, event_task);
    event_thread.detach();

    _event_running = true;

    /* zera o buffer de Frames */
    _frameBuffer->clear();
    
    MessageDealer::showMessage("ProcessingTask message: Processing thread created event and detached it.");
    MessageDealer::showMessage("ProcessingTask message: Tamaho do *_frameBuffer: " + std::to_string(_frameBuffer->size()));
}

void ProcessingTask::manageEvent() {
    std::time_t time_now = std::time(nullptr);

    event_task->addFrameToBuffer();

    if (_thereIsValidMotion) {
        _lastMotionDetectedTime = time_now;
        followDetectedMotion();
    }

    std::time_t event_duration = time_now - _eventStartTime;
    std::time_t time_without_motion = time_now - _lastMotionDetectedTime;

    bool no_motion_period = time_without_motion > EVENT_MAX_INACTIVITY_TIME;
    bool event_max_time_reached = event_duration > EVENT_MAX_DURATION;

    if (no_motion_period || event_max_time_reached) {
        finalizeEvent();
        MessageDealer::showMessage("Evento finalizado em " + getFormatedTime(time_now, "%H:%M:%S"));
    }

    if (event_max_time_reached) {
        startEvent();
    }
}

void ProcessingTask::finalizeEvent() {
    _event_running = false;
    event_task->finishEvent(_eventHorizontalDirection, _eventVerticalDirection, _eventMotionQuantity);
    event_task = nullptr;

    //    std::list<Event*>::iterator it = _eventsList->begin();
    //    while (it != _eventsList->end()) {
    //        Event *my_event = dynamic_cast<Event*> (*it);
    //        _eventsList->erase(it++); // alternatively, i = items.erase(i);
    //        MessageDealer::showMessage("Dados do evento]:" + my_event->toString());
    //        delete my_event;
    //    }
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
    _cap.read(_originalFrame);
    //    /* adiciona o frame lido no buffer de frames */
    //    _frameBuffer->pushBackFrame(_originalFrame);
    /* redimensiona o frame lido (por questões de desempenho,
     * somente o frame redimensionado sera usado para processamento) */
    cv::resize(_originalFrame, _prevFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
    /* converte para gray scale*/
    cvtColor(_prevFrame, _prevFrame, CV_BGR2GRAY);

    /* lê o segundo frame (frame corrente) */
    _cap.read(_originalFrame);
    //    _frameBuffer->pushBackFrame(_originalFrame);
    cv::resize(_originalFrame, _currentFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
    cvtColor(_currentFrame, _currentFrame, CV_BGR2GRAY);

    /* lê o terceiro frame (próximo frame) */
    _cap.read(_originalFrame);
    //    _frameBuffer->pushBackFrame(_originalFrame);
    cv::resize(_originalFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
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
        double numberOfChanges = 0;
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
                    numberOfChanges++;
                    if (_motion_min_x > i) _motion_min_x = i;
                    if (_motion_max_x < i) _motion_max_x = i;
                    if (_motion_min_y > j) _motion_min_y = j;
                    if (_motion_max_y < j) _motion_max_y = j;
                }
            }
        }

        if (numberOfChanges >= _minMotionValue) {
            _thereIsValidMotion = true;
            if (_event_running) {
                _eventMotionQuantity += numberOfChanges;
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
        if (_event_running) {
            _eventHorizontalDirection++;
        }
    } else {
        _horizontalDirection = left;
        if (_event_running) {
            _eventHorizontalDirection--;
        }
    }

    if (_previousMotionCenter.y < _motionCenter.y) {
        _verticalDirection = up;
        if (_event_running) {
            _eventVerticalDirection++;
        }
    } else {
        _verticalDirection = down;
        if (_event_running) {
            _eventVerticalDirection--;
        }
    }
}

void ProcessingTask::defineMotionQuadrant() {
    if (_motionCenter.x < DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y < DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = down_left;
    if (_motionCenter.x >= DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y < DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = down_right;
    if (_motionCenter.x >= DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y >= DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = up_right;
    if (_motionCenter.x < DEFAULT_PROCESSED_FRAME_CENTER.x && _motionCenter.y >= DEFAULT_PROCESSED_FRAME_CENTER.y) _quadrant = up_left;
}

void ProcessingTask::openAndConfigureVideoDevice() {
    try {
        // abre dispositivo de vídeo com o argumento passado (índice do dispositivo de vídeo)
        _cap.open(_capDeviceIndex);
        if (_cap.isOpened()) {
            // define o tamanho do frame (largura X altura)
            _cap.set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
            _cap.set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
        } else {
            _executionError = true;
            MessageDealer::showErrorMessage(std::string("Cannot open/configure the video device with index ") + std::to_string(_capDeviceIndex));
        }
    } catch (std::exception & e) {
        _executionError = true;
        MessageDealer::showErrorMessage(std::string("Error in processing task while openning video device: ") + std::string(e.what()));
    }
}

void ProcessingTask::estimateFPS() {
    try {
        int num_frames = 120; // número de frames para capturar no treino
        int cont = 0;
        time_t start, end; // tempo inicial e final

        time(&start); // define o tempo inicial

        /* fluxo normal de execução*/
        while (!_executionError && cont < num_frames) {

            _prevFrame = _currentFrame;
            _currentFrame = _nextFrame;

            _cap.read(_originalFrame);

            cv::resize(_originalFrame, _nextFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            _result = _nextFrame;

            cvtColor(_nextFrame, _nextFrame, CV_BGR2GRAY);

            cv::absdiff(_prevFrame, _nextFrame, _d1);
            cv::absdiff(_nextFrame, _currentFrame, _d2);

            cv::bitwise_and(_d1, _d2, _motion);

            cv::threshold(_motion, _motion, 35, 255, CV_THRESH_BINARY);

            cv::erode(_motion, _motion, _kernelErode);

            detectMotion();

            cont++;

            //            /* mostra os resultados da detecção de movimentos */
            //            cv::imshow("Motion", _motion);
            //            cv::imshow("Result", _result);

            if (cv::waitKey(30) >= 0) break;
        }

        time(&end); // define o tempo final

        double seconds = difftime(end, start); // tempo decorrido

        _fps = num_frames / seconds; // calcula a taxa de frames por segundo (aproximada)

        MessageDealer::showMessage("FPS estimado:" + std::to_string(_fps));
    } catch (std::exception & e) {
        _cap.release();
        _executionError = true;
        //        _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in processing task while treining FPS rate: ") + std::string(e.what()));
    }
}