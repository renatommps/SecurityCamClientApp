/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.h
 * Author: renato
 *
 * Created on 31 de Outubro de 2016, 15:54
 */

#ifndef EVENT_H
#define EVENT_H

#include <ctime>        //std::time_t
#include <time.h>       //time_t, time, ctime
#include <string>       //std::string
#include "MessageDealer.h"
class Event {
public:
    Event(std::string id, std::string video_path, std::string video_name, std::string video_extention, std::time_t start_time);
    Event(const Event& orig);
    virtual ~Event();

    std::string getId();
    std::string getVideoPath();
    std::string getVideoName();
    std::string getVideoFullName();
    std::time_t getStartTime();
    double getDuration();
    long getFramesQuantity();
    long getHorizontalDirection();
    long getVerticalDirection();
    double getMotionQuantity();
    
    void setDuration(double duration); // define a duração em segundos do evento
    void setFramesQuantity(long quantity); // define a quantidade acumulada de frames (quadros de vídeo) do evento
    void setMotionQuantity(double quantity); // define a quantidade de movimentação acumulada do evento
    void incrementFramesQuantity(); // incrementa de um a quantidade acumulada de frames
    void incrementMotionQuantity(double quantity); // incrementa a quantidade acumulada de movimentação do evento com a quantidade passada
    void incrementHorizontalDirection(int quantity); // incrementar de 1 significa incremento para a direita, e -1, para a esquerda;
    void incrementVerticalDirection(short int quantity); // incrementar de 1 significa incremento para cima, e -1, para baixo;
private:
    std::string _id; // identificação do evento
    std::string _videoPath; // caminho (do diretório/pasta) do vídeo que sera gravado do evento
    std::string _videoName;
    std::string _videoExtention; // extensão usada para salvar o vídeo do evento (Ex: .mp4, .avi, etc...)
    std::time_t _startTime;
    double _duration; // duração do evento em segundos;
    long _framesQuantity; // quantidade acumulada de frames (quadros de vídeo) do evento
    int _horizontalDirection; // valores negativos representam direção média de movimentos para a esquerda, e positivos para a direita;
    long _verticalDirection; // valores negativos representam direção média de movimentos para baixo, e positivos para cima;
    double _motionQuantity; // quantia acumulada de movimentação detectada do evento

};

#endif /* EVENT_H */

