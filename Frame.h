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
#include "opencv2/opencv.hpp"
#include <ctime>            // std::time_t
#include <time.h>           // time_t, time, ctime

class Frame {
public:
    Frame(cv::Mat frame, std::time_t time);
    Frame(const Frame& orig);
    virtual ~Frame();
    void write(cv::FileStorage& fs) const; // Write serialization for this class
    void read(const cv::FileNode& node); // Read serialization for this class
    cv::Mat getCvMat();
    std::time_t getTime();
private:
    cv::Mat _frame;
    std::time_t _time;
};

#endif /* FRAME_H */

