/* 
 * File:   bufferManager.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:50
 */

#include "BufferManager.h"

BufferManager::BufferManager() {
}

BufferManager::BufferManager(const BufferManager& orig) {
}

BufferManager::~BufferManager() {
}

void BufferManager::pushBackFrame(cv::Mat frame) {
    _bufferMutex.lock();
    _frameBuffer.push_back(frame);
    _bufferMutex.unlock();
}

cv::Mat BufferManager::popFrontFrame() {
    cv::Mat frame;

    _bufferMutex.lock();
    frame = _frameBuffer.front();
    _frameBuffer.pop_front();
    _bufferMutex.unlock();

    return frame;
}

bool BufferManager::empty() {
    bool empty;

    _bufferMutex.lock();
    empty = _frameBuffer.empty();
    _bufferMutex.unlock();

    return empty;
}

int BufferManager::size() {
    int size;

    _bufferMutex.lock();
    size = _frameBuffer.size();
    _bufferMutex.unlock();

    return size;
}