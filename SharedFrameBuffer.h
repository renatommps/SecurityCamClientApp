#ifndef SHAREDFRAMEBUFFER_H
#define SHAREDFRAMEBUFFER_H

#include <list>     // std::list
#include <mutex>    // std::mutex, std::unique_lock
#include "EventFrame.h"

class SharedFrameBuffer {
public:
    SharedFrameBuffer();
    SharedFrameBuffer(const SharedFrameBuffer& orig);
    virtual ~SharedFrameBuffer();
    void pushBackFrame(EventFrame * frame);
    void assign(std::list<EventFrame> * frameBuffer);
    void assign(SharedFrameBuffer * frameBuffer);
    EventFrame * popFrontFrame();
    EventFrame * getFrontFrame();
    std::list<EventFrame> getFrameBuffer();
    bool empty();
    int size();
private:
    std::list<EventFrame> _frameBuffer;
    std::mutex _bufferMutex;
};

#endif /* SHAREDFRAMEBUFFER_H */

