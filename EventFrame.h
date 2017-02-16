#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <ctime>            // std::time_t
#include <time.h>           // time_t, time, ctime

class EventFrame {
public:
    EventFrame(cv::Mat frame, std::time_t time);
    EventFrame(const EventFrame& orig);
    virtual ~EventFrame();
    cv::Mat getCvMat();
    std::time_t getTime();
private:
    cv::Mat _frame;
    std::time_t _time;
};

#endif /* EVENTFRAME_H */

