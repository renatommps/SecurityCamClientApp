#ifndef EVENTSTORAGETASK_H
#define EVENTSTORAGETASK_H

#include "SharedFrameBuffer.h"

class EventStorageTask {
public:
    EventStorageTask(SharedFrameBuffer *frameBuffer, bool *eventActive);
    EventStorageTask(const EventStorageTask& orig);
    virtual ~EventStorageTask();
    void start();
private:
    SharedFrameBuffer *_frameBuffer;
    bool *_eventActive;
};

#endif /* EVENTSTORAGETASK_H */

