#include "EventStorageTask.h"

EventStorageTask::EventStorageTask(SharedFrameBuffer *frameBuffer, bool *eventActive) {
    _frameBuffer->assign(frameBuffer);
    _eventActive = eventActive;
    _eventName = "test.xml";
}

EventStorageTask::EventStorageTask(const EventStorageTask& orig) {
}

EventStorageTask::~EventStorageTask() {
}

void EventStorageTask::start() {
    cv::FileStorage file("test.xml", cv::FileStorage::WRITE);
    file.release();

    while (_eventActive) {
        if (!_frameBuffer->empty()) {
            MessageDealer::showMessage(std::string("EventStorageTask message: ") + std::string("Frames to store: ") + std::to_string(_frameBuffer->size()));
            Frame frame = _frameBuffer->getFrontFrame();
            try {
                if (!file.isOpened()) {
                    file.open("test.xml", cv::FileStorage::APPEND);
                }
                file << "Frame" << frame;
            } catch (std::exception & e) {
                MessageDealer::showErrorMessage(std::string("Error in EventStorageTask: ") + std::string(e.what()));
                break;
            }
        } else {
            if (file.isOpened()) {
                file.release();
                MessageDealer::showMessage(std::string("EventStorageTask message: ") + std::string("Frame buffer empty."));
            }
        }
    }

    if (file.isOpened()) {
        file.release();
    }

    MessageDealer::showMessage(std::string("EventStorageTask finalizing: "));
}

std::string EventStorageTask::getExecutionPath() {
    std::string path = "";
//    int path_max = 1024;
//    char buff[path_max];
//    ssize_t len = readlink("/proc/self/exe", buff, sizeof ( buff) - 1);
//
//    if (len != -1) {
//        buff[len] = '\0';
//        size_t found;
//        std::string full_name = std::string(buff);
//        found = full_name.find_last_of("/\\");
//        path = full_name.substr(0, found);
//    }
    return path;
}