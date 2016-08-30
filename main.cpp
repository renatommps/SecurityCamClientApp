/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: renato
 *
 * Created on 17 de Agosto de 2016, 10:26
 */

// #include <stdlib.h>     // atoi
// #include <thread>       // std::thread
// #include <string>       // std::string
// #include <fstream>      // std::fstream
// #include <fstream>      // std::ofstream
// #include <unistd.h>	// std::usleep
// #include <chrono>	// std::chrono::milliseconds
// #include <opencv2/opencv.hpp>
// #include <zmq.hpp>
// #include "zhelpers.hpp"
//
// static void show_error_on_file_and_screen(std::string error);
// std::string get_selfpath();
// bool setClientName(std::string *c_name);
// static bool openAndConfigureVideoDevice(cv::VideoCapture *cap_device, int src);
// void readInicialFrame(cv::VideoCapture *cap_device, cv::Mat *f, cv::Mat *pf, cv::Mat *ppf);
// void setFrameParameters(cv::Mat *f, int *f_width, int *f_height, int *pf_width, int *pf_height, int *max_mov_width, int *min_mov_width,
// 			int *min_horizontal_mov_dist, int *min_vertical_mov_dist, int *pf_border_width);
// void defineServoPosition(short int servo_channel, short int position);
//
// const short int DEFAULT_WIDTH = 640; // width (largura) padão do frame
// const short int DEFAULT_HEIGHT = 480; // height (altura) padão do frame
// const short int DEFAULT_RESIZE_SCALE = 4; // fator usado para redimensionar o frame que sera processado (para aumentar a velocidade de processamento)
// const cv::Size DEFAULT_PROCESSED_FRAME_SIZE ( DEFAULT_WIDTH / DEFAULT_RESIZE_SCALE, DEFAULT_HEIGHT / DEFAULT_RESIZE_SCALE ); // tamanho do frame que sera processado
//
// const std::string SERVO_FILE = "/dev/servoblaster"; // arquivo onde deve ser gravada a posição do servo motor (usando )
// const short int SERVO_MIN_POSITION = 55; // posicao mínima que o servo motor pode assumir (posição da extrema direita)
// const short int SERVO_MAX_POSITION = 230; // posicao máxima que o servo motor pode assumir (posição da extrema esquerda)
// const short int SERVO_CENTRAL_POSITION = 120; //((SERVO_MAX_POSITION + SERVO_MIN_POSITION) / 2); // posição central do servo motor
// const short int SERVO_DEFAULT_STEP = 10; // tamanho normal do passo que o servo da a cada movimentação
// short int _servo_position = SERVO_CENTRAL_POSITION;
//
// std::string _executablePath;
//
// std::string _serverIP;
// std::string _serverPort; // = "5570";
// std::string _mac;
//
// cv::VideoCapture _cap;
// cv::Mat _frame;
// cv::Mat _processedFrame;
// cv::Mat _LastProcessedFrame;
// int _frameWidth;
// int _frameHeight;
// int _processedFrameWidth;
// int _processedFrameHeight;
// int _processedFrameBorderWidth;
// int _maximum_movement_width;
// int _minimal_movement_width;
// int _minimalHorizontalMomementDistance;
// int _minimalVerticalMomementDistance;
//
// class client_task {
// public:
//   client_task(std::string serverIP, std::string serverPort, std::string id) : ctx_(1), client_socket_(ctx_, ZMQ_DEALER), serverIP_(serverIP), serverPort_(serverPort), identity_(id) {} // constructor
//
//     void start() {
//
// 	std::cout << "client task started, task id: " << identity_ << ", server ip: " << serverIP_<< ", server port: " << serverPort_ << std::endl;
//
// 	// define socket
//         client_socket_.setsockopt(ZMQ_IDENTITY, identity_.c_str(), identity_.size()); 	// set socket ID
// 	client_socket_.connect(std::string("tcp://") + serverIP_ + std::string(":") + serverPort_); 		// set socket connection
//
// 	zmq::pollitem_t items[] = {{static_cast<void *>(client_socket_), 0, ZMQ_POLLIN, 0}}; //typedef struct {void //*socket//;int //fd (file descriptor)//;short //events//;short //revents//;} zmq_pollitem_t;
// 	int request_nbr = 0;
//
// 	try {
//
// 	  std::string frame_width  = std::to_string(_frameWidth);
// 	  std::string frame_heigth  = std::to_string(_frameHeight);
// 	  std::string matAsString (_frame.datastart, _frame.dataend);
//
// 	  zmq::message_t identity;
// 	  zmq::message_t msg;
//
// 	  s_sendmore(client_socket_, identity_);
// 	  s_sendmore(client_socket_, frame_width);
// 	  s_sendmore(client_socket_, frame_heigth);
// 	  s_send(client_socket_, matAsString);
//
//
// 	  int receiveTryes = 1;
// 	  bool msgReceived = false;
//
// 	  while (msgReceived == false ) {
//
// 	    zmq::poll(items, 1, 10); // int zmq_poll (zmq_pollitem_t *items, int nitems, long timeout (10 milliseconds) ); OBS: All ØMQ sockets passed to the zmq_poll() function must share the same ØMQ context and must belong to the thread calling zmq_poll().
// 	    if (items[0].revents & ZMQ_POLLIN) {
// 	      s_dump(client_socket_);
// 	      msgReceived = true;
// 	    } else {
// 	      std::cout << "vai dormir e tentar outra tentativa de receber uma resposta do server.. " << std::endl;
// 	      std::this_thread::sleep_for(std::chrono::milliseconds(500));
// 	    }
// 	  }
// 	}
// 	catch (std::exception &e) {
// 	  std::cout << "Error in client task : " << e.what() << std::endl;
// 	}
//
// 	std::cout << "Client thread finished execution!" << std::endl;
//     }
//
// private:
//     zmq::context_t ctx_;
//     zmq::socket_t client_socket_;
//     std::string serverIP_;
//     std::string serverPort_;
//     std::string identity_;
// };
//
//
// int main (int argc, char *argv[])
// {
//     if (argc < 4) { // É esperado 4 argumentos: o nome do programa (por padrão é passado),o índice do dispositivo de vídeo, o IP do server, e a porta do server
//       show_error_on_file_and_screen("Usage: " + std::string(argv[0]) + " <VIDEO DEVICE INDEX> <SERVER IP> <SERVER PORT>");
//       exit (1);
//     } else {
//       _executablePath = get_selfpath();
//       _serverIP = argv[2];
//       _serverPort = argv[3];
//       std::cout << "executablePath: " << _executablePath << ", server IP: " << _serverIP << ", server port: " << _serverPort << std::endl;
//     }
//
//     if ( !setClientName(&_mac) ) {
//       show_error_on_file_and_screen("MAC file doesn't find, it wasn't possible to define client name");
//       exit (1);
//     }
//
//     // abre dispositivo de vídeo com o argumento passado (índice do dispositivo de vídeo) e define o tamanho (largura X altura)
//     if ( !openAndConfigureVideoDevice( &_cap, atoi(argv[1]) ) ) {
//       show_error_on_file_and_screen("Cannot open/configure the video device with index " + std::string(argv[1]));
//     }
//
//     readInicialFrame(&_cap, &_frame, &_processedFrame, &_LastProcessedFrame);
//
//     setFrameParameters(&_frame, &_frameWidth, &_frameHeight, &_processedFrameWidth, &_processedFrameHeight, &_maximum_movement_width,
// 		       &_minimal_movement_width, &_minimalHorizontalMomementDistance, &_minimalVerticalMomementDistance, &_processedFrameBorderWidth);
//
//     defineServoPosition(2, SERVO_CENTRAL_POSITION);
//
//     if(_cap.isOpened()) {
//       _cap.release(); // release VideoCapture device
//     }
//
//     client_task ct(_serverIP, _serverPort, _mac);
//     std::thread t(&client_task::start, &ct);
//
//     std::cout << "Main execution will join client task and wait to finish it's execution!" << std::endl;
//     t.join();
//     std::cout << "Main execution finished, process terminated!" << std::endl;
//
//     return 0;
// }
//
// static void show_error_on_file_and_screen(std::string error)
// {
//   std::ofstream outFile("execution_error_log.txt");
//   outFile << error; // put on file
//   std::cerr << error << std::endl; // put on screen
// }
//
// std::string get_selfpath()
// {
//   std::string path = "";
//   int path_max = 1024;
//   char buff[path_max];
//   ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
//
//   if (len != -1) {
//     buff[len] = '\0';
//     size_t found;
//     std::string full_name = std::string(buff);
//     found = full_name.find_last_of("/\\");
//     path = full_name.substr(0,found);
//   }
//   return path;
// }
//
// bool setClientName(std::string *c_name)
// {
//   bool client_set = false;
//   std::ifstream infile("mac.txt");
//
//   if (infile.good()) {
//     std::getline(infile, *c_name);
//     client_set = true;
//   } else {
//     show_error_on_file_and_screen("Could'not read mac.txt file");
//   }
//   infile.close();
//
//   return client_set;
// }
//
// static bool openAndConfigureVideoDevice(cv::VideoCapture *cap_device, int src)
// {
//   cap_device->open(src);
//
//   if ( !cap_device->isOpened() ) {
//     return false;
//   }
//
//   cap_device->set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
//   cap_device->set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
//
//   return true;
// }
//
// void readInicialFrame(cv::VideoCapture *cap_device, cv::Mat *f, cv::Mat *pf, cv::Mat *ppf)
// {
//   cap_device->read( *f );
//   cv::resize(*f, *pf, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
//   *ppf = pf->clone();
// }
//
// void setFrameParameters(cv::Mat *f, int *f_width, int *f_height, int *pf_width, int *pf_height, int *max_mov_width,
// 			int *min_mov_width, int *min_horizontal_mov_dist, int *min_vertical_mov_dist, int *pf_border_width)
// {
//   *f_width = f->cols;
//   *f_height = f->rows;
//
//   *pf_width = *f_width / DEFAULT_RESIZE_SCALE;
//   *pf_height = *f_height / DEFAULT_RESIZE_SCALE;
//
//   *pf_border_width = (int) *pf_width/4;
//
//   *max_mov_width = (*pf_width / 2.5);  // 40% do tamanho total da imagem (tamanho/100)*40 = tamanho / 2.5
//   *min_mov_width = (*pf_width / 20); // 5% do tamanho total da imagem (tamanho/100)*5 = tamanho / 20
//
//   *min_horizontal_mov_dist = *pf_width/10; // a distancia horizontal percorrida entre detecções deve ser de no mínimo 10% do tamanho horzontal da imagem processada
//   *min_vertical_mov_dist = *pf_height/10; // a distancia vertical percorrida entre detecções deve ser de no mínimo 10% do tamanho vertical da imagem processada
//
//   std::cout << "Processed Frame size = " << *pf_width << "x" << *pf_height << ", area = " << (*pf_width) * (*pf_height) << std::endl;
// }
//
// void defineServoPosition(short int servo_channel, short int position)
// {
//   std::stringstream ss;
//   std::ofstream servo(SERVO_FILE);
//   std::string write;
//
//   if ( position > SERVO_MAX_POSITION ) {
//     _servo_position = SERVO_MAX_POSITION;
//   } else if ( position < SERVO_MIN_POSITION ) {
//     _servo_position = SERVO_MIN_POSITION;
//   } else {
//     _servo_position = position;
//   }
//
//   ss.str("");
//   ss << servo_channel << "=" << _servo_position << "\n";
//   write = ss.str();
//
//   servo << write;
//   servo.close();
//   cv::waitKey(30);
// }

#include <stdlib.h>         // atoi
#include <thread>           // std::thread
#include <string>           // std::string
#include <fstream>          // std::fstream
#include <fstream>          // std::ofstream
#include <list>             // std::list
#include <unistd.h>         // std::usleep
#include <chrono>           // std::chrono::milliseconds
#include <mutex>               // std::mutex, std::unique_lock
#include <condition_variable>  // std::condition_variable
#include <opencv2/opencv.hpp>
#include <zmq.hpp>

#include "zhelpers.hpp"

static void show_error_on_file_and_screen(std::string error);
std::string get_selfpath();
bool setClientName(std::string * c_name);
void readInicialFrame(cv::VideoCapture * cap_device, cv::Mat * f, cv::Mat * pf, cv::Mat * ppf);
void setFrameParameters(cv::Mat * f, int * f_width, int * f_height, int * pf_width, int * pf_height, int * max_mov_width, int * min_mov_width,
        int * min_horizontal_mov_dist, int * min_vertical_mov_dist, int * pf_border_width);
void defineServoPosition(short int servo_channel, short int position);
static bool ProcessingTaskInNormalFunction();


const short int DEFAULT_WIDTH = 640; // width (largura) padão do frame
const short int DEFAULT_HEIGHT = 480; // height (altura) padão do frame
const short int DEFAULT_RESIZE_SCALE = 4; // fator usado para redimensionar o frame que sera processado (para aumentar a velocidade de processamento)
const cv::Size DEFAULT_PROCESSED_FRAME_SIZE(DEFAULT_WIDTH / DEFAULT_RESIZE_SCALE, DEFAULT_HEIGHT / DEFAULT_RESIZE_SCALE); // tamanho do frame que sera processado

const std::string SERVO_FILE = "/dev/servoblaster"; // arquivo onde deve ser gravada a posição do servo motor (usando )
const short int SERVO_MIN_POSITION = 55; // posicao mínima que o servo motor pode assumir (posição da extrema direita)
const short int SERVO_MAX_POSITION = 230; // posicao máxima que o servo motor pode assumir (posição da extrema esquerda)
const short int SERVO_CENTRAL_POSITION = 120; //((SERVO_MAX_POSITION + SERVO_MIN_POSITION) / 2); // posição central do servo motor
const short int SERVO_DEFAULT_STEP = 10; // tamanho normal do passo que o servo da a cada movimentação
short int _servo_position = SERVO_CENTRAL_POSITION;

std::string _executablePath;
int _capDeviceIndex;
std::string _serverIP;
std::string _serverPort; // = "5570";
std::string _mac;

cv::VideoCapture _cap;
cv::Mat _frame;
cv::Mat _processedFrame;
cv::Mat _clientFrame;
cv::Mat _LastProcessedFrame;
std::list<cv::Mat> frame_buffer;
int _frameWidth;
int _frameHeight;
int _processedFrameWidth;
int _processedFrameHeight;
int _processedFrameBorderWidth;
int _maximum_movement_width;
int _minimal_movement_width;
int _minimalHorizontalMomementDistance;
int _minimalVerticalMomementDistance;

bool errorOnProcessingTask = false;
bool ProcessingTaskDone = false;
bool errorOnClientTask = false;
bool clientThreadNotified = false;
bool clientTaskInitializad = false;

std::condition_variable cond_var;
std::mutex variable_access_mutex;
std::mutex sincronization_mutex;
std::mutex clientFrame_mutex;
std::mutex buffer_mutex;

class processing_task {
    
    bool openAndConfigureVideoDevice();
    
public:

    processing_task(int capDeviceIndex) : capDeviceIndex_(capDeviceIndex) {
    } // constructor

    void start() {
        std::cout << "Processing task started." << std::endl;

        /* abre o dispositivo de vídeo*/
        errorOnProcessingTask = openAndConfigureVideoDevice();

        /* espera o Client task iniciar (sincroniza) */
        try {
            bool clientStartReady = false;
            while (!clientStartReady) {
                std::lock_guard<std::mutex> lk(sincronization_mutex);
                clientStartReady = clientTaskInitializad;
            }
        } catch (std::exception & e) {
            std::cout << "Error in processing task : " << e.what() << std::endl;
            errorOnProcessingTask = true;
        }

        try {
            _cap.read(_frame);

            buffer_mutex.lock();
            frame_buffer.push_back(_frame);
            buffer_mutex.unlock();

            cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

            int cont = 0;
            /* fluxo normal de execução*/
            while (!errorOnProcessingTask && cont < 1000) {

                _LastProcessedFrame = _processedFrame.clone();
                _cap.read(_frame);

                buffer_mutex.lock();
                frame_buffer.push_back(_frame);
                buffer_mutex.unlock();

                cv::resize(_frame, _processedFrame, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);

                cont++;
            }

            variable_access_mutex.lock();
            ProcessingTaskDone = true;
            variable_access_mutex.unlock();

        } catch (std::exception & e) {
            std::cout << "Error in processing task : " << e.what() << std::endl;
            errorOnProcessingTask = true;
        }

        std::cout << "Processing thread finished execution!" << std::endl;
    }



private:

    bool openAndConfigureVideoDevice(cv::VideoCapture * cap_device, int src) {
        bool statusError = false;

        try {
            // abre dispositivo de vídeo com o argumento passado (índice do dispositivo de vídeo)
            cap_device->open(src);
            if (cap_device->isOpened()) {
                // define o tamanho do frame (largura X altura)
                cap_device->set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
                cap_device->set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
            } else {
                statusError = true;
                show_error_on_file_and_screen("Cannot open/configure the video device with index " + _capDeviceIndex);
            }
        } catch (std::exception & e) {
            statusError = true;
            show_error_on_file_and_screen( std::string("Error in processing task : ") + std::string(e.what()) );
        }
        return statusError;
    }

    int capDeviceIndex_;
    cv::VideoCapture cap_;
};

class client_task {
public:

    client_task(std::string serverIP, std::string serverPort, std::string id) : ctx_(1), client_socket_(ctx_, ZMQ_DEALER), serverIP_(serverIP), serverPort_(serverPort), identity_(id) {
    } // constructor

    void start() {
        std::cout << "client task started, task id: " << identity_ << ", server ip: " << serverIP_ << ", server port: " << serverPort_ << std::endl;
        time_t now;
        tm *ltm;

        //        /* inicia o socket */
        //        try {
        //            // define socket
        //            client_socket_.setsockopt(ZMQ_IDENTITY, identity_.c_str(), identity_.size()); // set socket ID
        //            client_socket_.connect(std::string("tcp://") + serverIP_ + std::string(":") + serverPort_); // set socket connection
        //
        //            zmq::pollitem_t items[] = {
        //                {static_cast<void *> (client_socket_), 0, ZMQ_POLLIN, 0}
        //            }; //typedef struct {void //*socket//;int //fd (file descriptor)//;short //events//;short //revents//;} zmq_pollitem_t;
        //
        //            std::lock_guard<std::mutex> lk(sincronization_mutex);
        //            clientTaskInitializad = true;
        //        } catch (std::exception & e) {
        //            std::cout << "Error in client task : " << e.what() << std::endl;
        //            errorOnProcessingTask = true;
        //        }
        //
        //        /* ciclo normal de execução*/
        //        try {
        //            while (ProcessingTaskInNormalFunction()) {
        //
        //
        //
        //
        //            }
        //
        //        } catch
        //            (std::exception & e) {
        //            std::cout << "Error in client task : " << e.what() << std::endl;
        //        }

        std::cout << "Client thread finished execution!" << std::endl;
    }

    bool notified() {
        variable_access_mutex.lock();
        bool status = !clientThreadNotified && !ProcessingTaskDone;
        variable_access_mutex.unlock();
        return status;
    }

private:
    zmq::context_t ctx_;
    zmq::socket_t client_socket_;
    std::string serverIP_;
    std::string serverPort_;
    std::string identity_;
};

class storage_tast {
public:

    storage_tast() {
    } // constructor

    void start() {
        std::cout << "Storage task started" << std::endl;

        /* ciclo normal de execução*/
        try {
            while (ProcessingTaskInNormalFunction()) {
                while (frameBufferEmpty()) {
                    std::cout << "Frame buffer empty, storage task waiting" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }

                /* lê o primeiro elemento da fila e o remove */
                variable_access_mutex.lock();
                std::cout << "There are " << frame_buffer.size() << " frames to store." << std::endl;
                storage_frame = frame_buffer.front();
                frame_buffer.pop_front();
                variable_access_mutex.unlock();

                cv::imshow("Frame", storage_frame);
                if (cv::waitKey(30) >= 0) break;
            }

        } catch
            (std::exception & e) {
            std::cout << "Error in Storage task : " << e.what() << std::endl;
        }

        std::cout << "Storage thread finished execution!" << std::endl;
    }

private:
    cv::Mat storage_frame;

    bool frameBufferEmpty() {
        bool frameEmpty;
        buffer_mutex.lock();
        frameEmpty = frame_buffer.empty();
        buffer_mutex.unlock();

        return frameEmpty;
    }
};

static bool ProcessingTaskInNormalFunction() {
    variable_access_mutex.lock();
    bool status = !errorOnProcessingTask && !ProcessingTaskDone;
    variable_access_mutex.unlock();
    return status;
}

int main(int argc, char * argv[]) {
    if (argc < 4) { // É esperado 4 argumentos: o nome do programa (por padrão é passado),o índice do dispositivo de vídeo, o IP do server, e a porta do server
        show_error_on_file_and_screen("Usage: " + std::string(argv[0]) + " <VIDEO DEVICE INDEX> <SERVER IP> <SERVER PORT>");
        exit(1);

    } else {
        _executablePath = get_selfpath();
        _capDeviceIndex = atoi(argv[1]);
        _serverIP = argv[2];
        _serverPort = argv[3];
        std::cout << "executablePath: " << _executablePath << ", capture device index: " << _capDeviceIndex << ", server IP: " << _serverIP << ", server port: " << _serverPort << std::endl;
    }

    if (!setClientName(&_mac)) {
        show_error_on_file_and_screen("MAC file doesn't find, it wasn't possible to define client name");
        exit(1);
    }

    processing_task pt(_capDeviceIndex);
    //storage_tast st();
    client_task ct(_serverIP, _serverPort, _mac);

    std::thread tp(&processing_task::start, &pt);
    //std::thread ts(&storage_tast::start, &st);
    std::thread tc(&client_task::start, &ct);

    std::cout << "Main execution will join client task and wait to finish it's execution!" << std::endl;
    tp.join();
    //ts.join();
    tc.join();
    std::cout << "Main execution finished, process terminated!" << std::endl;

    return 0;
}

static void show_error_on_file_and_screen(std::string error) {
    std::ofstream outFile("execution_error_log.txt");
    outFile << error; // put on file
    std::cerr << error << std::endl; // put on screen
}

std::string get_selfpath() {
    std::string path = "";
    int path_max = 1024;
    char buff[path_max];
    ssize_t len = readlink("/proc/self/exe", buff, sizeof ( buff) - 1);

    if (len != -1) {
        buff[len] = '\0';
        size_t found;
        std::string full_name = std::string(buff);
        found = full_name.find_last_of("/\\");
        path = full_name.substr(0, found);
    }

    return path;
}

bool setClientName(std::string * c_name) {
    bool client_set = false;
    std::ifstream infile("mac.txt");

    if (infile.good()) {
        std::getline(infile, *c_name);
        client_set = true;

    } else {
        show_error_on_file_and_screen("Could'not read mac.txt file");
    }

    infile.close();

    return client_set;
}

// static bool openAndConfigureVideoDevice(cv::VideoCapture *cap_device, int src)
// {
//   cap_device->open(src);
//
//   if ( !cap_device->isOpened() ) {
//     return false;
//   }
//
//   cap_device->set(CV_CAP_PROP_FRAME_WIDTH, DEFAULT_WIDTH);
//   cap_device->set(CV_CAP_PROP_FRAME_HEIGHT, DEFAULT_HEIGHT);
//
//   return true;
// }

void readInicialFrame(cv::VideoCapture * cap_device, cv::Mat * f, cv::Mat * pf, cv::Mat * ppf) {
    cap_device->read(*f);
    cv::resize(*f, *pf, DEFAULT_PROCESSED_FRAME_SIZE, 0, 0, CV_INTER_AREA);
    *ppf = pf->clone();
}

void setFrameParameters(cv::Mat * f, int * f_width, int * f_height, int * pf_width, int * pf_height, int * max_mov_width,
        int * min_mov_width, int * min_horizontal_mov_dist, int * min_vertical_mov_dist, int * pf_border_width) {
    *f_width = f->cols;
    *f_height = f->rows;

    *pf_width = *f_width / DEFAULT_RESIZE_SCALE;
    *pf_height = *f_height / DEFAULT_RESIZE_SCALE;

    *pf_border_width = (int) * pf_width / 4;

    *max_mov_width = (*pf_width / 2.5); // 40% do tamanho total da imagem (tamanho/100)*40 = tamanho / 2.5
    *min_mov_width = (*pf_width / 20); // 5% do tamanho total da imagem (tamanho/100)*5 = tamanho / 20

    *min_horizontal_mov_dist = *pf_width / 10; // a distancia horizontal percorrida entre detecções deve ser de no mínimo 10% do tamanho horzontal da imagem processada
    *min_vertical_mov_dist = *pf_height / 10; // a distancia vertical percorrida entre detecções deve ser de no mínimo 10% do tamanho vertical da imagem processada

    std::cout << "Processed Frame size = " << *pf_width << "x" << *pf_height << ", area = " << (*pf_width) * (*pf_height) << std::endl;
}

void defineServoPosition(short int servo_channel, short int position) {
    std::stringstream ss;
    std::ofstream servo(SERVO_FILE);
    std::string write;

    if (position > SERVO_MAX_POSITION) {
        _servo_position = SERVO_MAX_POSITION;

    } else if (position < SERVO_MIN_POSITION) {
        _servo_position = SERVO_MIN_POSITION;

    } else {
        _servo_position = position;
    }

    ss.str("");
    ss << servo_channel << "=" << _servo_position << "\n";
    write = ss.str();

    servo << write;
    servo.close();
    cv::waitKey(30);
}

