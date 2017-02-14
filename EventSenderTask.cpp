#include "EventSenderTask.h"

EventSenderTask::EventSenderTask(SharedFrameBuffer *frameBuffer, bool *eventActive) {
   _frameBuffer->assign(frameBuffer);
    _eventActive = eventActive;
}

EventSenderTask::EventSenderTask(const EventSenderTask& orig) {
}

EventSenderTask::~EventSenderTask() {
}

void EventSenderTask::start() {

    while (_eventActive) {

    }
}