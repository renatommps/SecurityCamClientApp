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

void SynchronizationAndStatusDealer::setProcessingTaskErrorStatus(bool status){
    _ProcessingTaskErrorStatusMutex.lock();
    _ProcessingTaskErrorStatus = status;
    _ProcessingTaskErrorStatusMutex.unlock();
}

bool SynchronizationAndStatusDealer::getProcessingTaskErrorStatus(){
    bool status;
    _ProcessingTaskErrorStatusMutex.lock();
    status = _ProcessingTaskErrorStatus;
    _ProcessingTaskErrorStatusMutex.unlock();
    
    return status;
}