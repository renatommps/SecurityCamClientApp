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
    void setProcessingTaskExecutionStatus(bool status);
    bool ProcessingTaskHasError();
    bool ProcessingTaskExecuting();
    bool getStorageTaskErrorStatus();
    bool TasksHasError();
    void setMotionEventStatus(bool status);
    bool getMotionEventStatus();
private:
    std::mutex _ProcessingTaskStatusMutex;
    std::mutex _StorageTaskErrorStatusMutex;
    std::mutex _MotionEventStatusMutex;
    bool _ProcessingTaskErrorStatus;
    bool _ProcessingTaskExecutionStatus;
    bool _StorageTaskErrorStatus;
    bool _motionEventStatus;
};

#endif /* SYNCHRONIZATIONANDSTATUSDEALER_H */

