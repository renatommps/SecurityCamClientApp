/* 
 * File:   storageTask.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 12:11
 */

#include "StorageTask.h"

StorageTask::StorageTask() {
}

StorageTask::StorageTask(BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer) :
_frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
} // constructor

StorageTask::~StorageTask() {
}

void StorageTask::start() {
    MessageDealer::showMessage("Storage task started.");
    cv::namedWindow("Frame", CV_WINDOW_AUTOSIZE);

    try {
        while (!_synchAndStatusDealer->getTasksErrorStatus()) {
            if (!_frameBuffer->empty()) {
                MessageDealer::showMessage("Frame buffer size: " + std::to_string(_frameBuffer->size()));
                cv::Mat frame = _frameBuffer->popFrontFrame();

                if (!frame.empty()) {
                    cv::imshow("frame", frame);
                    if (cv::waitKey(30) >= 0) break;
                } else {
                    _synchAndStatusDealer->setStorageTaskErrorStatus(true);
                    cv::destroyWindow("Frame"); //destroy the window with the name, "Frame"
                    MessageDealer::showMessage("Frame empty! Some thing got wrong");
                }
            } else {
                MessageDealer::showMessage("Frame buffer empty, Storage task waiting...");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    } catch (std::exception & e) {
        cv::destroyWindow("Frame"); //destroy the window with the name, "Frame"
        _synchAndStatusDealer->setStorageTaskErrorStatus(true);
        MessageDealer::showErrorMessage(std::string("Error in Storage task : ") + std::string(e.what()));
    }
    cv::destroyWindow("Frame"); //destroy the window with the name, "Frame"
    MessageDealer::showMessage("Storage thread finished execution!");
}