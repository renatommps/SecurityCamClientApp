/* 
 * File:   synchronizationAndStatusDealer.h
 * Author: renato
 *
 * Created on 31 de Agosto de 2016, 11:05
 */

#ifndef SYNCHRONIZATIONANDSTATUSDEALER_H
#define SYNCHRONIZATIONANDSTATUSDEALER_H

#include <mutex>                // std::mutex, std::unique_lock

class SynchronizationAndStatusDealer {
public:
    SynchronizationAndStatusDealer();
    SynchronizationAndStatusDealer(const SynchronizationAndStatusDealer& orig);
    virtual ~SynchronizationAndStatusDealer();
    void setProcessingTaskErrorStatus(bool status);
    void setStorageTaskErrorStatus(bool status);
    bool getProcessingTaskErrorStatus();
    bool getStorageTaskErrorStatus();
    bool getTasksErrorStatus();
private:
    std::mutex _ProcessingTaskErrorStatusMutex;
    std::mutex _StorageTaskErrorStatusMutex;
    bool _ProcessingTaskErrorStatus;
    bool _StorageTaskErrorStatus;
};

#endif /* SYNCHRONIZATIONANDSTATUSDEALER_H */

