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

class BufferManager {
public:
    BufferManager();
    BufferManager(const BufferManager& orig);
    virtual ~BufferManager();
    void pushBackFrame(cv::Mat frame);
    cv::Mat popFrontFrame();
private:
    std::list<cv::Mat> _frameBuffer;
    std::mutex _bufferMutex;
};

#endif /* BUFFERMANAGER_H */

