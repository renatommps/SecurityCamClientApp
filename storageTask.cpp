/* 
 * File:   storageTask.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 12:11
 */

#include "storageTask.h"

storageTask::storageTask() {
}

storageTask::storageTask(bufferManager *buffer, synchronizationAndStatusDealer *synchAndStatusDealer) :
_frameBuffer(buffer), _synchAndStatusDealer(synchAndStatusDealer) {
} // constructor

storageTask::~storageTask() {
}

void storageTask::start() {
    //messageDealer::showMessage("Storage task started.");
}