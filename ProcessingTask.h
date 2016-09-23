/* 
 * File:   processing_task.h
 * Author: renato
 *
 * Created on 30 de Agosto de 2016, 15:00
 */

#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H

#include <string>       //std::string
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "MessageDealer.h"
#include "BufferManager.h"
#include "SynchronizationAndStatusDealer.h"

const int DELAY = 30; // em milissegundos, delay entre frames
const short int DEFAULT_WIDTH = 640; // width (largura) padão do frame
const short int DEFAULT_HEIGHT = 480; // height (altura) padão do frame
const short int DEFAULT_RESIZE_SCALE = 1; // fator usado para redimensionar o frame que sera processado (para aumentar a velocidade de processamento)
const cv::Size DEFAULT_PROCESSED_FRAME_SIZE(DEFAULT_WIDTH / DEFAULT_RESIZE_SCALE, DEFAULT_HEIGHT / DEFAULT_RESIZE_SCALE); // tamanho do frame que sera processado
const cv::Point FRAME_CENTER(DEFAULT_PROCESSED_FRAME_SIZE.width/2, DEFAULT_PROCESSED_FRAME_SIZE.height/2);

enum Horizontal_direction {
    left, right
};

enum Vertical_direction {
    down, up
};

enum Quadrant {
    down_left, down_right, up_left, up_right
};

class ProcessingTask {
public:
    ProcessingTask();
    ProcessingTask(int capDeviceIndex, BufferManager *buffer, SynchronizationAndStatusDealer *synchAndStatusDealer);
    virtual ~ProcessingTask();
    void start();
private:
    bool _executionError;
    int _capDeviceIndex;
    
    cv::VideoCapture _cap;
    cv::Mat _rawFrame;
    cv::Mat _prevFrame;
    cv::Mat _currentFrame;
    cv::Mat _nextFrame;
    cv::Mat _d1; // diferença entre _prevFrame e _nextFrame
    cv::Mat _d2; // diferença entre _nextFrame e _currentFrame
    cv::Mat _result; // resultado da operação, calculado sobre _d1 e _d2
    cv::Mat resultCropped;
    cv::Mat _motion;
    cv::Mat _kernelErode; // kernel de erosão
    
    int _numberOfChanges; // número de mudanças na matrix _result
    int _thereIsMotion; // se existe mais que '_thereIsMotion' pixels que mudaram, então é considerado que existe movimento
    int _maxDeviation; // desvio máximo da imagem, quanto maior o valor, mais movimento é aceito (motion)
    int _numberOfConsecutiveMotionSequence;
    int _motion_min_x;
    int _motion_min_y;
    int _motion_max_x;
    int _motion_max_y;
    
    Horizontal_direction _horizontalDirection;
    Vertical_direction _verticalDirection;
    Quadrant _quadrant;
    
    cv::Point _previousMotionCenter;
    cv::Point _motionCenter;
    cv::Rect _motion_rectangle;
    cv::Scalar _mean;
    cv::Scalar _color; // amarelo, a cor usada para desenhar um retangulo quando alguma coisa mudou (movimento)
    
    BufferManager *_frameBuffer;
    SynchronizationAndStatusDealer *_synchAndStatusDealer;

    bool openAndConfigureVideoDevice();
    int detectMotion();
    void resetBackgroundModelFrames();
    void defineMotionDirection();
    void defineMotionQuadrant();
};

#endif /* PROCESSINGTASK_H */

