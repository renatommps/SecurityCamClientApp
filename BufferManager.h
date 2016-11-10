/* 
 * File:   bufferManager.h
 * Author: renato
 *
 * Created on 30 de Agosto de 2016, 15:50
 */

#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <list>                 // std::list
#include <mutex>                // std::mutex, std::unique_lock
#include <opencv2/opencv.hpp>   // cv::Mat
#include "Frame.h"

const int BUFFER_MAX_SIZE = 10; // tamanho máximo do buffer de frames

class BufferManager {
public:
    BufferManager();
    BufferManager(const BufferManager& orig);
    virtual ~BufferManager();
    void pushBackFrame(cv::Mat frame);
    void pushBackFrame(cv::Mat frame, Type type);
    cv::Mat popFrontFrame();
    bool empty();
    int size();
private:
    std::list<Frame> _frameBuffer;
    std::mutex _bufferMutex;
};

#endif /* BUFFERMANAGER_H */

