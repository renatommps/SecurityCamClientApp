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
}