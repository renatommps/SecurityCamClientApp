/* 
 * File:   bufferManager.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:50
 */

#include "bufferManager.h"

bufferManager::bufferManager() {
}

bufferManager::bufferManager(const bufferManager& orig) {
}

bufferManager::~bufferManager() {
}

void bufferManager::pushBackFrame(cv::Mat frame) {
    _bufferMutex.lock();
    _frameBuffer.push_back(frame);
    _bufferMutex.unlock();
}

cv::Mat bufferManager::popFrontFrame() {

}
