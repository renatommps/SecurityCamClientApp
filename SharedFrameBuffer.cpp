#include "SharedFrameBuffer.h"

SharedFrameBuffer::SharedFrameBuffer() {
}

SharedFrameBuffer::SharedFrameBuffer(const SharedFrameBuffer& orig) {
}

SharedFrameBuffer::~SharedFrameBuffer() {
}

void SharedFrameBuffer::pushBackFrame(Frame frame) {
    Frame f(frame);
    
    _bufferMutex.lock();
    _frameBuffer.push_back(f);
    _bufferMutex.unlock();
}

Frame SharedFrameBuffer::popFrontFrame() {
    cv::Mat frame;

    _bufferMutex.lock();
    if (!_frameBuffer.empty()) {
        frame = _frameBuffer.front();
        _frameBuffer.pop_front();
    }
    _bufferMutex.unlock();

    return frame;
}

Frame SharedFrameBuffer::getFrontFrame() {
    cv::Mat frame;

    _bufferMutex.lock();
    if (!_frameBuffer.empty()) {
        frame = _frameBuffer.front();
    }
    _bufferMutex.unlock();

    return frame;
}

bool SharedFrameBuffer::empty() {
    bool empty;

    _bufferMutex.lock();
    empty = _frameBuffer.empty();
    _bufferMutex.unlock();

    return empty;
}

int SharedFrameBuffer::size() {
    int size;

    _bufferMutex.lock();
    size = _frameBuffer.size();
    _bufferMutex.unlock();

    return size;
}