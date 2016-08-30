/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processing_task.h
 * Author: renato
 *
 * Created on 30 de Agosto de 2016, 15:00
 */

#ifndef PROCESSING_TASK_H
#define PROCESSING_TASK_H

#include <string>       //std::string
#include "messageDealer.h"

class processing_task {
public:
    processing_task();
    processing_task(int capDeviceIndex);
    virtual ~processing_task();
    void start();
private:
    int _capDeviceIndex;
    cv::VideoCapture _cap;
    
    bool openAndConfigureVideoDevice();
};

#endif /* PROCESSING_TASK_H */

