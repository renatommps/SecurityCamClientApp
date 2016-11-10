/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.cpp
 * Author: renato
 * 
 * Created on 31 de Outubro de 2016, 15:54
 */

#include "Event.h"

Event::Event(std::string id, std::string video_path, std::string video_name, std::string video_extention, std::time_t start_time, double fps, cv::Size size) :
_id(id), _videoPath(video_path), _videoName(video_name), _videoExtention(video_extention), _startTime(start_time),
_framesQuantity(0), _horizontalDirection(0), _verticalDirection(0), _motionQuantity(0.0), _eventSavedStatus(false),
_fps(fps), _videoSize(size) {
    _videoWriter.open(
            (video_path + "/" + video_name + video_extention), // Name of the output video file.
            VIDEO_WRITER_CODEC, // fourcc – 4 - character code of codec used to compress the frames. For example, CV_FOURCC('P', 'I', 'M', '1') is a MPEG - 1 codec, CV_FOURCC('M', 'J', 'P', 'G') is a motion - jpeg codec etc.
            fps, // fps – Framerate of the created video stream.
            size, // frameSize – Size of the video frames.
            true); // isColor – If it is not zero, the encoder will expect and encode color frames, otherwise it will work with grayscale frames(the flag is currently supported on Windows only).

}

Event::Event(const Event& orig) {
}

Event::~Event() {
    if (getEventSavedStatus()) {
        if (std::remove(getVideoFullName().c_str()) != 0) {
            MessageDealer::showErrorMessage("Error deleting video file! File '" + getVideoFullName() + "' dosn't exist.");
        } else {
            MessageDealer::showMessage("File '" + getVideoFullName() + "' successfully deleted!");
        }
    } else {
        if (std::remove(getVideoFullName().c_str()) != 0) {
            MessageDealer::showErrorMessage("File'" + getVideoFullName() + "' not saved and not found!'");
        } else {
            MessageDealer::showMessage("File '" + getVideoFullName() + "' found but is not saved, will not be deleted!");
        }
    }
}

std::string Event::getId() {
    return _id;
}

std::string Event::getVideoPath() {
    return _videoPath;
}

std::string Event::getVideoName() {
    return _videoName;
}

std::string Event::getVideoFullName() {
    return _videoPath + "/" + _videoName + _videoExtention;
}

std::time_t Event::getStartTime() {
    return _startTime;
}

double Event::getDuration() {
    return _duration;
}

long Event::getFramesQuantity() {
    return _framesQuantity;
}

long Event::getHorizontalDirection() {
    return _horizontalDirection;
}

long Event::getVerticalDirection() {
    return _verticalDirection;
}

double Event::getMotionQuantity() {
    return _motionQuantity;
}

std::string Event::getFormatedTime(std::time_t raw_time, std::string format) {
    struct tm * timeinfo = localtime(&raw_time);
    char buffer [80];
    strftime(buffer, 80, format.c_str(), timeinfo);
    std::string str_time(buffer);

    return str_time;
}

bool Event::getEventSavedStatus() {
    return _eventSavedStatus;
}

void Event::setDuration(double duration) {
    _duration = duration;
}

void Event::setFramesQuantity(long quantity) {
    _framesQuantity = quantity;
}

void Event::setMotionQuantity(double quantity) {
    _motionQuantity = quantity;
}

void Event::setEventSavedStatus(bool status) {
    _eventSavedStatus = status;
}

void Event::incrementFramesQuantity() {
    _framesQuantity++;
    MessageDealer::showMessage("frames quantity: " + std::to_string(_framesQuantity));
}

void Event::incrementMotionQuantity(double quantity) {
    _motionQuantity += quantity;
}

void Event::incrementHorizontalDirection(int quantity) {
    _horizontalDirection += quantity;
}

void Event::incrementVerticalDirection(short int quantity) {
    _verticalDirection += quantity;
}

bool Event::saveInfoOnDisk() {

}

std::string Event::toString() {
    std::string eventString = (
            "\nID: " + getId() +
            "\nStart time: " + getFormatedTime(getStartTime(), "%H:%M:%S") +
            "\nDuratioin: " + std::to_string(getDuration()) +
            "\nFrames quantity: " + std::to_string(getFramesQuantity()) +
            "\nMotion Quantity: " + std::to_string(getMotionQuantity()) +
            "\nHorizontal Direction: " + std::to_string(getHorizontalDirection()) +
            "\nVertical Direction: " + std::to_string(getVerticalDirection()) +
            "\nVideo full name: " + getVideoFullName() +
            "\n");
    return eventString;
}