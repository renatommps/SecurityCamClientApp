/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processing_task.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:00
 */

#include "processing_task.h"

processing_task::processing_task() {
}

processing_task::processing_task(int capDeviceIndex) : _cap(capDeviceIndex) {
} // constructor

processing_task::~processing_task() {
}

void start() {
    std::cout << "Processing task started." << std::endl;

    /* abre o dispositivo de vídeo*/
    errorOnProcessingTask = openAndConfigureVideoDevice();

    /* espera o Client task iniciar (sincroniza) */
    try {
        bool clientStartReady = false;
        while (!clientStartReady) {
            std::lock_guard<std::mutex> lk(sincronization_mutex);
            clientStartReady = clientTaskInitializad;
        }
    } catch (std::exception & e) {
        showErrorMessage(std::string("Error in processing task : ") + std::string(e.what()) );
        errorOnProcessingTask = true;
    }

    try {
        _cap.read(_frame);

        buffer_mutex.lock();
        frame_buffer.push_back(_frame);
        buffer_mutex.unlock();
        
        cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

        int cont = 0;
        /* fluxo normal de execução*/
        while (!errorOnProcessingTask && cont < 1000) {

            _LastProcessedFrame = _processedFrame.clone();
            _cap.read(_frame);

            buffer_mutex.lock();
            frame_buffer.push_back(_frame);
            buffer_mutex.unlock();

            cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            cont++;
        }

        variable_access_mutex.lock();
        ProcessingTaskDone = true;
        variable_access_mutex.unlock();

    } catch (std::exception & e) {
        std::cout << "Error in processing task : " << e.what() << std::endl;
        errorOnProcessingTask = true;
    }

    std::cout << "Processing thread finished execution!" << std::endl;
}

bool openAndConfigureVideoDevice(cv::VideoCapture * cap_device, int src) {
    bool statusError = false;

    try {
        // abre dispositivo de vídeo com o argumento passado (índice do dispositivo de vídeo)
        cap_device->open(src);
        if (cap_device->isOpened()) {
            // define o tamanho do frame (largura X altura)
            cap_device->set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
            cap_device->set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
        } else {
            statusError = true;
            show_error_on_file_and_screen("Cannot open/configure the video device with index " + _capDeviceIndex);
        }
    } catch (std::exception & e) {
        statusError = true;
        show_error_on_file_and_screen(std::string("Error in processing task : ") + std::string(e.what()));
    }
    return statusError;
}