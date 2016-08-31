/* 
 * File:   processing_task.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:00
 */

#include "processingTask.h"

processingTask::processingTask() {
}

processingTask::processingTask(int capDeviceIndex, bufferManager *buffer, synchronizationAndStatusDealer *synchAndStatusDealer) :
_cap(capDeviceIndex), _frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
} // constructor

processingTask::~processingTask() {
}

void processingTask::start() {
    //messageDealer::showMessage("Processing task started.");

    /* abre o dispositivo de vídeo*/
    _executionError = openAndConfigureVideoDevice();

    while (!_executionError) {
        try {
            /* lê frame do dispositivo de vídeo */
            _cap.read(_frame);

            /* adiciona o frame lido no buffer de frames */
            _frameBuffer->pushBackFrame(_frame);

            /* redimensiona o frame lido (por questões de desempenho,
             * somente o frame redimensionado sera usado para processamento) */
            cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            int cont = 0;
            /* fluxo normal de execução*/
            while (!_executionError && cont < 1000) {

                /* atualiza o ultimo frame lido como sendo o frame atual
                 * (frames redimensionados, usado para processamento) */
                _LastProcessedFrame = _processedFrame.clone();

                /* lê frame do dispositivo de vídeo */
                _cap.read(_frame);

                /* adiciona o frame lido no buffer de frames */
                _frameBuffer->pushBackFrame(_frame);

                /* redimensiona o frame lido (por questões de desempenho,
                 * somente o frame redimensionado sera usado para processamento) */
                cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);


                /* FAZ O PROCESSAMENTO AQUI EM DIANTE!*/



                cont++;
            }
            _executionError = true;
            _synchAndStatusDealer->setProcessingTaskErrorStatus(_executionError);
        } catch (std::exception & e) {
            _executionError = true;
            _synchAndStatusDealer->setProcessingTaskErrorStatus(true);
            //messageDealer::showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()));
        }
    }
    //messageDealer::showMessage("Processing thread finished execution!");
}

bool processingTask::openAndConfigureVideoDevice() {
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
            //messageDealer::showErrorMessage( std::string("Cannot open/configure the video device with index ") + std::to_string(_capDeviceIndex) );
        }
    } catch (std::exception & e) {
        statusError = true;
        //messageDealer::showErrorMessage( std::string("Error in processing task : ") + std::string(e.what()) );
    }
    return statusError;
}