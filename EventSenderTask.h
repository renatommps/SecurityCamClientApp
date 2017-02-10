#ifndef EVENTSENDERTASK_H
#define EVENTSENDERTASK_H

#include "SharedFrameBuffer.h"

class EventSenderTask {
public:
    EventSenderTask(SharedFrameBuffer *frameBuffer, bool *eventActive);
    EventSenderTask(const EventSenderTask& orig);
    virtual ~EventSenderTask();
    void start();
private:
    SharedFrameBuffer *_frameBuffer;
    bool *_eventActive;
};

#endif /* EVENTSENDERTASK_H */

