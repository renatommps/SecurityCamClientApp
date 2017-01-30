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

Frame::Frame(cv::Mat frame, std::time_t time): _frame(frame), _time(time) {
}

Frame::Frame(const Frame& orig) {
}

Frame::~Frame() {
}

cv::Mat Frame::getCvMat() {
    return _frame;
}

std::time_t Frame::getTime() {
    return _time;
}
