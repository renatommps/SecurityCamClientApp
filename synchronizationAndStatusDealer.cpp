/* 
 * File:   synchronizationAndStatusDealer.cpp
 * Author: renato
 * 
 * Created on 31 de Agosto de 2016, 11:05
 */

#include "synchronizationAndStatusDealer.h"

synchronizationAndStatusDealer::synchronizationAndStatusDealer() {
    _ProcessingTaskErrorStatus = false;
}

synchronizationAndStatusDealer::synchronizationAndStatusDealer(const synchronizationAndStatusDealer& orig) {
    _ProcessingTaskErrorStatus = false;
}

synchronizationAndStatusDealer::~synchronizationAndStatusDealer() {
}

void synchronizationAndStatusDealer::setProcessingTaskErrorStatus(bool status){
    _ProcessingTaskErrorStatusMutex.lock();
    _ProcessingTaskErrorStatus = status;
    _ProcessingTaskErrorStatusMutex.unlock();
}

bool synchronizationAndStatusDealer::getProcessingTaskErrorStatus(){
    bool status;
    _ProcessingTaskErrorStatusMutex.lock();
    status = _ProcessingTaskErrorStatus;
    _ProcessingTaskErrorStatusMutex.unlock();
    
    return status;
}