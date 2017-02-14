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

Frame::Frame(cv::Mat frame, std::time_t time) : _frame(frame), _time(time) {
}

Frame::Frame(const Frame& orig) {
}

Frame::~Frame() {
}

void Frame::write(cv::FileStorage& fs) const // Write serialization for this class
{
    fs << "{" << "Frame" << _frame << "Time" << _time << "}";
}

void Frame::read(const cv::FileNode& node) // Read serialization for this class
{
    _frame = (cv::Mat) node["Frame"];
    _time = (std::time_t) node["Time"];
}

cv::Mat Frame::getCvMat() {
    return _frame;
}

std::time_t Frame::getTime() {
    return _time;
}

//These write and read functions must be defined for the serialization in FileStorage to work

static void write(cv::FileStorage& fs, const std::string&, const Frame& x) {
    x.write(fs);
}

static void read(const cv::FileNode& node, Frame& x, const Frame& default_value = Frame()) {
    if (node.empty())
        x = default_value;
    else
        x.read(node);
}