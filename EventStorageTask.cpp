#include "EventStorageTask.h"

EventStorageTask::EventStorageTask(SharedFrameBuffer *frameBuffer, bool *eventActive) {
    _frameBuffer->assign(frameBuffer);
    _eventActive = eventActive;
}

EventStorageTask::EventStorageTask(const EventStorageTask& orig) {
}

EventStorageTask::~EventStorageTask() {
}

void EventStorageTask::start() {

    while (_eventActive) {
        
        
        
        
        
    }
}
