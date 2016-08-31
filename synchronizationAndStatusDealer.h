/* 
 * File:   synchronizationAndStatusDealer.h
 * Author: renato
 *
 * Created on 31 de Agosto de 2016, 11:05
 */

#ifndef SYNCHRONIZATIONANDSTATUSDEALER_H
#define SYNCHRONIZATIONANDSTATUSDEALER_H

#include <mutex>                // std::mutex, std::unique_lock


class synchronizationAndStatusDealer {
public:
    synchronizationAndStatusDealer();
    synchronizationAndStatusDealer(const synchronizationAndStatusDealer& orig);
    virtual ~synchronizationAndStatusDealer();
    void setProcessingTaskErrorStatus(bool status);
    bool getProcessingTaskErrorStatus();
private:
    std::mutex _ProcessingTaskErrorStatusMutex;
    bool _ProcessingTaskErrorStatus;
};

#endif /* SYNCHRONIZATIONANDSTATUSDEALER_H */

