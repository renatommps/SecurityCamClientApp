/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Frame.cpp
 * Author: renato
 * 
 * Created on 9 de Novembro de 2016, 13:00
 */

#include "Frame.h"

Frame::Frame(cv::Mat frame, Type type) : _frame(frame), _type(type) {
}

Frame::Frame(cv::Mat frame) : _frame(frame), _type(OUTSIDE_EVENT) {
}

Frame::Frame(const Frame& orig) {
}

Frame::~Frame() {
}

cv::Mat Frame::getFrame() {
    return _frame;
}

Type Frame::getType() {
    return _type;
}
