/* 
 * File:   synchronizationAndStatusDealer.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 11:05
 */

#include "SynchronizationAndStatusDealer.h"

SynchronizationAndStatusDealer::SynchronizationAndStatusDealer() {
    _ProcessingTaskErrorStatus = false;
    _motionEventStatus = false;
}

SynchronizationAndStatusDealer::SynchronizationAndStatusDealer(const SynchronizationAndStatusDealer& orig) {
    _ProcessingTaskErrorStatus = false;
    _motionEventStatus = false;
}

SynchronizationAndStatusDealer::~SynchronizationAndStatusDealer() {
}

void SynchronizationAndStatusDealer::setProcessingTaskErrorStatus(bool status) {
    _ProcessingTaskStatusMutex.lock();
    _ProcessingTaskErrorStatus = status;
    _ProcessingTaskStatusMutex.unlock();
}

void SynchronizationAndStatusDealer::setProcessingTaskExecutionStatus(bool status) {
    _ProcessingTaskStatusMutex.lock();
    _ProcessingTaskExecutionStatus = status;
    _ProcessingTaskStatusMutex.unlock();
}

void SynchronizationAndStatusDealer::setStorageTaskErrorStatus(bool status) {
    _StorageTaskErrorStatusMutex.lock();
    _StorageTaskErrorStatus = status;
    _StorageTaskErrorStatusMutex.unlock();
}

bool SynchronizationAndStatusDealer::ProcessingTaskHasError() {
    bool status;

    _ProcessingTaskStatusMutex.lock();
    status = _ProcessingTaskErrorStatus;
    _ProcessingTaskStatusMutex.unlock();

    return status;
}

bool SynchronizationAndStatusDealer::ProcessingTaskExecuting() {
    bool status;

    _ProcessingTaskStatusMutex.lock();
    status = _ProcessingTaskExecutionStatus;
    _ProcessingTaskStatusMutex.unlock();

    return status;
}

bool SynchronizationAndStatusDealer::getStorageTaskErrorStatus() {
    bool status;

    _StorageTaskErrorStatusMutex.lock();
    status = _StorageTaskErrorStatus;
    _StorageTaskErrorStatusMutex.unlock();

    return status;
}

bool SynchronizationAndStatusDealer::TasksHasError() {
    bool status;

    std::lock(_ProcessingTaskStatusMutex, _StorageTaskErrorStatusMutex);
    std::lock_guard<std::mutex> lk1(_ProcessingTaskStatusMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lk2(_StorageTaskErrorStatusMutex, std::adopt_lock);

    status = _ProcessingTaskErrorStatus && _StorageTaskErrorStatus;

    return status;
}

void SynchronizationAndStatusDealer::setMotionEventStatus(bool status) {
    _MotionEventStatusMutex.lock();
    _motionEventStatus = status;
    _MotionEventStatusMutex.unlock();
}

bool SynchronizationAndStatusDealer::getMotionEventStatus() {
    bool status;

    _MotionEventStatusMutex.lock();
    status = _motionEventStatus;
    _MotionEventStatusMutex.unlock();

    return status;
}
