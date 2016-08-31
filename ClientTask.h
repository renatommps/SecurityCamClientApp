/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClientTask.h
 * Author: renato
 *
 * Created on 31 de Agosto de 2016, 16:29
 */

#ifndef CLIENTTASK_H
#define CLIENTTASK_H

#include <string>       //std::string
#include "MessageDealer.h"
#include "BufferManager.h"
#include "SynchronizationAndStatusDealer.h"

class ClientTask {
public:
    ClientTask();
    ClientTask(std::string mac, std::string serverIP, std::string serverPort,
            BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer);
    virtual ~ClientTask();
    void start();
private:
    std::string _id;
    std::string _serverIP;
    std::string _serverPort;
    BufferManager *_frameBuffer;
    SynchronizationAndStatusDealer *_synchAndStatusDealer;
};

#endif /* CLIENTTASK_H */

