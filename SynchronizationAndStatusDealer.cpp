/* 
 * File:   synchronizationAndStatusDealer.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 11:05
 */

#include "SynchronizationAndStatusDealer.h"

SynchronizationAndStatusDealer::SynchronizationAndStatusDealer() {
    _ProcessingTaskErrorStatus = false;
}

SynchronizationAndStatusDealer::SynchronizationAndStatusDealer(const SynchronizationAndStatusDealer& orig) {
    _ProcessingTaskErrorStatus = false;
}

SynchronizationAndStatusDealer::~SynchronizationAndStatusDealer() {
}

void SynchronizationAndStatusDealer::setProcessingTaskErrorStatus(bool status) {
    _ProcessingTaskErrorStatusMutex.lock();
    _ProcessingTaskErrorStatus = status;
    _ProcessingTaskErrorStatusMutex.unlock();
}

void SynchronizationAndStatusDealer::setStorageTaskErrorStatus(bool status) {
    _StorageTaskErrorStatusMutex.lock();
    _StorageTaskErrorStatus = status;
    _StorageTaskErrorStatusMutex.unlock();
}

bool SynchronizationAndStatusDealer::getProcessingTaskErrorStatus() {
    bool status;

    _ProcessingTaskErrorStatusMutex.lock();
    status = _ProcessingTaskErrorStatus;
    _ProcessingTaskErrorStatusMutex.unlock();

    return status;
}

bool SynchronizationAndStatusDealer::getStorageTaskErrorStatus() {
    bool status;

    _StorageTaskErrorStatusMutex.lock();
    status = _StorageTaskErrorStatus;
    _StorageTaskErrorStatusMutex.unlock();

    return status;
}

bool SynchronizationAndStatusDealer::getTasksErrorStatus() {
    bool status;

    std::lock(_ProcessingTaskErrorStatusMutex, _StorageTaskErrorStatusMutex);
    std::lock_guard<std::mutex> lk1(_ProcessingTaskErrorStatusMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lk2(_StorageTaskErrorStatusMutex, std::adopt_lock);

    status = _ProcessingTaskErrorStatus && _StorageTaskErrorStatus;
    
    return status;
}

