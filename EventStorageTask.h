#ifndef EVENTSTORAGETASK_H
#define EVENTSTORAGETASK_H

//#include <opencv2/opencv.hpp>
//#include "opencv2/highgui/highgui.hpp"
//#include <unistd.h>             // std::usleep, readlink
#include <string>
#include "SharedFrameBuffer.h"
#include "Frame.h"
#include "MessageDealer.h"

class EventStorageTask {
public:
    EventStorageTask(SharedFrameBuffer *frameBuffer, bool *eventActive);
    EventStorageTask(const EventStorageTask& orig);
    virtual ~EventStorageTask();
    void start();
private:
    std::string getExecutionPath();
    SharedFrameBuffer *_frameBuffer;
    bool *_eventActive;
    std::string _eventName;
};

#endif /* EVENTSTORAGETASK_H */

