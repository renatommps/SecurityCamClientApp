#ifndef CLIENTTASK_H
#define CLIENTTASK_H

#include <string>       //std::string
#include "MessageDealer.h"
#include "SharedFrameBuffer.h"
#include "SynchronizationAndStatusDealer.h"

class ClientTask {
public:
    ClientTask();
//    ClientTask(std::string mac, std::string serverIP, std::string serverPort,
//            SharedFrameBuffer *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer);
    virtual ~ClientTask();
    void start();
private:
    std::string _id;
    std::string _serverIP;
    std::string _serverPort;
    SharedFrameBuffer *_frameBuffer;
    SynchronizationAndStatusDealer *_synchAndStatusDealer;
};

#endif /* CLIENTTASK_H */

