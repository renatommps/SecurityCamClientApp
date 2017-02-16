#include "EventFrame.h"

EventFrame::EventFrame(cv::Mat frame, std::time_t time) : _frame(frame), _time(time) {
}

EventFrame::EventFrame(const EventFrame& orig) {
}

EventFrame::~EventFrame() {
}

cv::Mat EventFrame::getCvMat() {
    return _frame;
}

std::time_t EventFrame::getTime() {
    return _time;
}