#include "SharedFrameBuffer.h"

SharedFrameBuffer::SharedFrameBuffer() {
}

SharedFrameBuffer::SharedFrameBuffer(const SharedFrameBuffer& orig) {
}

SharedFrameBuffer::~SharedFrameBuffer() {
}

void SharedFrameBuffer::assign(std::list<EventFrame> * frameBuffer) {
    _bufferMutex.lock();
    if (!frameBuffer->empty()) {
        _frameBuffer = *frameBuffer;
    }
    _bufferMutex.unlock();
}

void SharedFrameBuffer::assign(SharedFrameBuffer * frameBuffer) {
    _bufferMutex.lock();
    if (!frameBuffer->empty()) {
        _frameBuffer = frameBuffer->getFrameBuffer();
    }
    _bufferMutex.unlock();
}

void SharedFrameBuffer::pushBackFrame(EventFrame * frame) {
    _bufferMutex.lock();
    _frameBuffer.push_back(*frame);
    _bufferMutex.unlock();
}

EventFrame * SharedFrameBuffer::popFrontFrame() {
     EventFrame * frame = nullptr;

    _bufferMutex.lock();
    if (!_frameBuffer.empty()) {
        frame = &_frameBuffer.front();
        _frameBuffer.pop_front();
    }
    _bufferMutex.unlock();

    return frame;
}

EventFrame * SharedFrameBuffer::getFrontFrame() {
    EventFrame * frame = nullptr;

    _bufferMutex.lock();
    if (!_frameBuffer.empty()) {
        frame = &_frameBuffer.front();
    }
    _bufferMutex.unlock();

    return frame;
}

std::list<EventFrame> SharedFrameBuffer::getFrameBuffer() {
    std::list<EventFrame> frameBuffer;

    _bufferMutex.lock();
    frameBuffer = _frameBuffer;
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