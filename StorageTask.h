/* 
 * File:   storageTask.h
 * Author: renato
 *
 * Created on 31 de Agosto de 2016, 12:11
 */

#ifndef STORAGETASK_H
#define STORAGETASK_H

#include "MessageDealer.h"
#include "BufferManager.h"
#include "SynchronizationAndStatusDealer.h"

class StorageTask {
public:
    StorageTask();
    StorageTask(BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer);
    virtual ~StorageTask();
    void start();
private:
    BufferManager *_frameBuffer;
    SynchronizationAndStatusDealer *_synchAndStatusDealer;
};

#endif /* STORAGETASK_H */

