#ifndef EVENT_H
#define EVENT_H

#include <opencv2/opencv.hpp>
#include "opencv/highgui.h"
#include <thread>               // std::thread
#include <list>                 // std::list
#include <ctime>                // std::time_t
#include <time.h>               // time_t, time, ctime
#include <string>               // std::string
#include "MessageDealer.h"
#include "SharedFrameBuffer.h"
#include "EventStorageTask.h"
#include "EventSenderTask.h"

const int VIDEO_WRITER_CODEC = CV_FOURCC('M', 'J', 'P', 'G'); // {'F','M','P','4'}; // codec utilizado pelo VideoWriter

class Event {
public:
    Event(std::list<EventFrame> *frameBuffer);
    Event(const Event& orig);
    virtual ~Event();
    void start();

    std::string getFormatedTime(std::time_t raw_time, std::string format);
    void addFrameToBuffer(EventFrame frame);
    void incrementMotionQuantity(double quantity);
    void finishEvent(int horizontalDirection, int verticalDirection, double motionQuantity);

    //    std::string getId();
    //    std::string getVideoPath();
    //    std::string getVideoName();
    //    std::string getVideoFullName();
    //    std::time_t getStartTime();
    //    double getDuration();
    //    long getFramesQuantity();
    //    long getHorizontalDirection();
    //    long getVerticalDirection();
    //    double getMotionQuantity();
    //    bool getEventSavedStatus();
    //    void setDuration(double duration); // define a duração em segundos do evento
    //    void setFramesQuantity(long quantity); // define a quantidade acumulada de frames (quadros de vídeo) do evento
    //    void setMotionQuantity(double quantity); // define a quantidade de movimentação acumulada do evento
    //    void setEventSavedStatus(bool status); // seta se o evento foi enviado com sucesso para o servidor
    //    void incrementFramesQuantity(); // incrementa de um a quantidade acumulada de frames
    //    void incrementMotionQuantity(double quantity); // incrementa a quantidade acumulada de movimentação do evento com a quantidade passada
    //    void incrementHorizontalDirection(int quantity); // incrementar de 1 significa incremento para a direita, e -1, para a esquerda;
    //    void incrementVerticalDirection(short int quantity); // incrementar de 1 significa incremento para cima, e -1, para baixo;
    //    std::string getFormatedTime(std::time_t raw_time, std::string format);
    //    bool saveInfoOnDisk(); // salva os dados do evento em disco para ser recuperado posteriormente
    //    std::string toString();
private:
    std::string _id; // identificação do evento
    std::string _directoryPath; // caminho (do diretório/pasta) para gravar o evento em disco
    std::time_t _startTime;
    double _duration; // duração do evento em segundos;
    long _framesQuantity; // quantidade acumulada de frames (quadros de vídeo) do evento
    double _fps; // frames por segundo do evento
    int _horizontalDirection; // valores negativos representam direção média de movimentos para a esquerda, e positivos para a direita;
    long _verticalDirection; // valores negativos representam direção média de movimentos para baixo, e positivos para cima;
    double _motionQuantity; // quantia acumulada de movimentação detectada do evento
    cv::Size _frameSize;
    bool _eventActive;

    SharedFrameBuffer _storageFrameBuffer;
    SharedFrameBuffer _senderFrameBuffer;

    //    std::list<Frame> _frameBuffer;
    //    SharedFrameBuffer _sharedFrameBuffer;
};

#endif /* EVENT_H */

