#include "SharedFrameBuffer.h"

SharedFrameBuffer::SharedFrameBuffer() {
}

SharedFrameBuffer::SharedFrameBuffer(const SharedFrameBuffer& orig) {
}

SharedFrameBuffer::~SharedFrameBuffer() {
}

void SharedFrameBuffer::assign(std::list<Frame> * frameBuffer) {
    if (!frameBuffer->empty()) {
        _frameBuffer.assign(frameBuffer);
    }
}

void SharedFrameBuffer::assign(SharedFrameBuffer * frameBuffer) {
    if (!frameBuffer->empty()) {
        _frameBuffer.assign(frameBuffer->getFrameBuffer());
    }
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

std::list<Frame> SharedFrameBuffer::getFrameBuffer() {
    std::list<Frame> frameBuffer;

    _bufferMutex.lock();
    frameBuffer.assign(_frameBuffer);
    _bufferMutex.unlock();
    
    return frameBuffer;
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