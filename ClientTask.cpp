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
    MessageDealer::showMessage("Storage task started.");
}