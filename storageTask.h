/* 
 * File:   storageTask.h
 * Author: renato
 *
 * Created on 31 de Agosto de 2016, 12:11
 */

#ifndef STORAGETASK_H
#define STORAGETASK_H

#include "messageDealer.h"
#include "bufferManager.h"
#include "synchronizationAndStatusDealer.h"

class storageTask {
public:
    storageTask();
    storageTask(bufferManager *buffer, synchronizationAndStatusDealer *synchAndStatusDealer);
    virtual ~storageTask();
    void start();
private:
    bufferManager *_frameBuffer;
    synchronizationAndStatusDealer *_synchAndStatusDealer;
};

#endif /* STORAGETASK_H */

