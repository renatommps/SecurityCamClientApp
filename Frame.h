/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Frame.h
 * Author: renato
 *
 * Created on 9 de Novembro de 2016, 13:00
 */

#ifndef FRAME_H
#define FRAME_H

#include "opencv2/highgui/highgui.hpp"

enum Type {
    OUTSIDE_EVENT,
    EVENT_START,
    INSIDE_EVENT,
    EVENT_END
};

class Frame {
public:
    Frame(cv::Mat frame, Type type);
    Frame(cv::Mat frame);
    Frame(const Frame& orig);
    virtual ~Frame();
    
    cv::Mat getFrame();
    Type getType();
private:
    cv::Mat _frame;
    Type _type; // indica se o frame não pertence a um evento, inicia um evento, esta contido em um evento, ou termina um evento
};

#endif /* FRAME_H */

