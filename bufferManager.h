/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bufferManager.h
 * Author: renato
 *
 * Created on 30 de Agosto de 2016, 15:50
 */

#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <list>             // std::list

class bufferManager {
public:
    bufferManager();
    bufferManager(const bufferManager& orig);
    virtual ~bufferManager();
    void pushBackFrame(cv::Mat frame);
    cv::Mat popFrontFrame();
private:
    std::list<cv::Mat> frameBuffer;
};

#endif /* BUFFERMANAGER_H */

