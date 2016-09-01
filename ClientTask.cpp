/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClientTask.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 16:29
 */

#include "ClientTask.h"

ClientTask::ClientTask() {
}

ClientTask::ClientTask(std::string mac, std::string serverIP, std::string serverPort,
        BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer) :
_id(mac), _serverIP(serverIP), _serverPort(serverPort), _frameBuffer(buffer),
_synchAndStatusDealer(synchAndStatusDealer) {
} // constructor

ClientTask::~ClientTask() {
}

void ClientTask::start() {
    MessageDealer::showMessage("Client task started.");
    
    try {
        while (!_synchAndStatusDealer->getTasksErrorStatus()) {
            if (!_frameBuffer->empty()) {
                
                if (!frame.empty()) {
  
                } else {
                    _synchAndStatusDealer->setClientTaskErrorStatus(true);
                    MessageDealer::showMessage("Client frame empty! Some thing got wrong");
                }
            } else {
                MessageDealer::showMessage("Client frame buffer empty, Client task waiting...");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    } catch (std::exception & e) {
        cv::destroyWindow("Frame"); //destroy the window with the name, "Frame"
        _synchAndStatusDealer->setStorageTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in Storage task : ") + std::string(e.what()));
    }
    
    MessageDealer::showMessage("Client thread finished execution!");
}