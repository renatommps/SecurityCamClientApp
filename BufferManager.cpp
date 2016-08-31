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

}
