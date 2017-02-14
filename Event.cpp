#include "Event.h"
#include "Frame.h"

Event::Event(std::list<Frame> *frameBuffer) {
    _startTime = frameBuffer->front().getTime();
    _id = getFormatedTime(_startTime, "%Y-%m-%d-%H-%M-%S");
    
    _frameSize.height = frameBuffer->front().getCvMat().cols;
    _frameSize.width = frameBuffer->front().getCvMat().rows;
//    _frameSize(frameBuffer->front().getCvMat().cols, frameBuffer->front().getCvMat().rows);
    _framesQuantity = frameBuffer->size();
    _horizontalDirection = 0;
    _verticalDirection = 0;
    _motionQuantity = 0;
    _eventActive = true;

    _storageFrameBuffer.assign(frameBuffer);
    _senderFrameBuffer.assign(frameBuffer);

    MessageDealer::showMessage("Event message: Evento instanciado em " + getFormatedTime(_startTime, "%H:%M:%S"));
    MessageDealer::showMessage("Event message: Quantidade de frames passados: " + std::to_string(_framesQuantity));
    MessageDealer::showMessage("Event message: Tamaho do buffer passado para o evento: " + std::to_string(frameBuffer->size()));
}

Event::Event(const Event& orig) {
}

Event::~Event() {
    //    if (getEventSavedStatus()) {
    //        if (std::remove(getVideoFullName().c_str()) != 0) {
    //            MessageDealer::showErrorMessage("Error deleting video file! File '" + getVideoFullName() + "' dosn't exist.");
    //        } else {
    //            MessageDealer::showMessage("File '" + getVideoFullName() + "' successfully deleted!");
    //        }
    //    } else {
    //        if (std::remove(getVideoFullName().c_str()) != 0) {
    //            MessageDealer::showErrorMessage("File'" + getVideoFullName() + "' not saved and not found!'");
    //        } else {
    //            MessageDealer::showMessage("File '" + getVideoFullName() + "' found but is not saved, will not be deleted!");
    //        }
    //    }
}

void Event::start() {

    EventStorageTask eventStorageTask(&_storageFrameBuffer, &_eventActive);
    EventSenderTask eventSenderTask(&_senderFrameBuffer, &_eventActive);

    std::thread _eventStorageTask(&EventStorageTask::start, &eventStorageTask);
    std::thread _eventSenderTask(&EventSenderTask::start, &eventSenderTask);

    _eventStorageTask.join();
    _eventSenderTask.join();

    while (_eventActive) {

    }

    while (!_storageFrameBuffer.empty() && !_senderFrameBuffer.empty()) {

    }
}

void Event::finishEvent(int horizontalDirection, int verticalDirection, double motionQuantity) {
    _horizontalDirection = horizontalDirection;
    _verticalDirection = verticalDirection;
    _motionQuantity = motionQuantity;

    _eventActive = false;
}

std::string Event::getFormatedTime(std::time_t raw_time, std::string format) {
    struct tm * timeinfo = localtime(&raw_time);
    char buffer [80];
    strftime(buffer, 80, format.c_str(), timeinfo);
    std::string str_time(buffer);

    return str_time;
}

void Event::addFrameToBuffer(Frame frame) {
    _storageFrameBuffer.pushBackFrame(frame);
    _senderFrameBuffer.pushBackFrame(frame);
    
    _framesQuantity++;
}

void Event::incrementMotionQuantity(double quantity) {
    _motionQuantity += quantity;
}

//std::string Event::getId() {
//    return _id;
//}
//
//std::string Event::getVideoPath() {
//    return _directoryPath;
//}
//
//std::string Event::getVideoName() {
//    return _videoName;
//}
//
//std::string Event::getVideoFullName() {
//    return _directoryPath + "/" + _videoName + _videoExtention;
//}
//
//std::time_t Event::getStartTime() {
//    return _startTime;
//}
//
//double Event::getDuration() {
//    return _duration;
//}
//
//long Event::getFramesQuantity() {
//    return _framesQuantity;
//}
//
//long Event::getHorizontalDirection() {
//    return _horizontalDirection;
//}
//
//long Event::getVerticalDirection() {
//    return _verticalDirection;
//}
//
//double Event::getMotionQuantity() {
//    return _motionQuantity;
//}
//
//std::string Event::getFormatedTime(std::time_t raw_time, std::string format) {
//    struct tm * timeinfo = localtime(&raw_time);
//    char buffer [80];
//    strftime(buffer, 80, format.c_str(), timeinfo);
//    std::string str_time(buffer);
//
//    return str_time;
//}
//
//bool Event::getEventSavedStatus() {
//    return _eventSavedStatus;
//}
//
//void Event::setDuration(double duration) {
//    _duration = duration;
//}
//
//void Event::setFramesQuantity(long quantity) {
//    _framesQuantity = quantity;
//}
//
//void Event::setMotionQuantity(double quantity) {
//    _motionQuantity = quantity;
//}
//
//void Event::setEventSavedStatus(bool status) {
//    _eventSavedStatus = status;
//}
//
//void Event::incrementFramesQuantity() {
//    _framesQuantity++;
//    MessageDealer::showMessage("frames quantity: " + std::to_string(_framesQuantity));
//}
//
//void Event::incrementMotionQuantity(double quantity) {
//    _motionQuantity += quantity;
//}
//
//void Event::incrementHorizontalDirection(int quantity) {
//    _horizontalDirection += quantity;
//}
//
//void Event::incrementVerticalDirection(short int quantity) {
//    _verticalDirection += quantity;
//}
//
//bool Event::saveInfoOnDisk() {
//
//}
//
//std::string Event::toString() {
//    std::string eventString = (
//            "\nID: " + getId() +
//            "\nStart time: " + getFormatedTime(getStartTime(), "%H:%M:%S") +
//            "\nDuratioin: " + std::to_string(getDuration()) +
//            "\nFrames quantity: " + std::to_string(getFramesQuantity()) +
//            "\nMotion Quantity: " + std::to_string(getMotionQuantity()) +
//            "\nHorizontal Direction: " + std::to_string(getHorizontalDirection()) +
//            "\nVertical Direction: " + std::to_string(getVerticalDirection()) +
//            "\nVideo full name: " + getVideoFullName() +
//            "\n");
//    return eventString;
//}