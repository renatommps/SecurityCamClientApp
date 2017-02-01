#include "Event.h"
#include "Frame.h"

Event::Event(SharedFrameBuffer frameBuffer, std::list<Frame> *_frameBuffer, double initialMotionQuantity) {
    _startTime = _frameBuffer->front().getTime();
    _id = getFormatedTime(_startTime, "%Y-%m-%d-%H-%M-%S");
    _frameSize = _frameBuffer->front().getCvMat().size;
    _framesQuantity = _frameBuffer->size();
    _horizontalDirection = 0;
    _verticalDirection = 0;
    _motionQuantity = 0;
    _eventActive = true;

    _sharedFrameBuffer.assign(frameBuffer);

    MessageDealer::showMessage("Evento instanciado em " + getFormatedTime(_startTime, "%H:%M:%S"));
    MessageDealer::showMessage("Tamaho do buffer de frames do evento: " + std::to_string(_frameBuffer->size()));
    MessageDealer::showMessage("Tamaho de _framesQuantity do evento: " + std::to_string(_framesQuantity));
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

    while (_eventActive) {
        
        
        
    }

    while (!_sharedFrameBuffer.empty()){
        
    }
    
    //    EventStorageTask _eventStorageTask;
    //    EventTransferTask _eventTransferTask;
}

void Event::finishEvent() {
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
    _sharedFrameBuffer.pushBackFrame(frame);
    _framesQuantity++;
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