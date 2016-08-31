/* 
 * File:   messageDealer.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:14
 */

#include "MessageDealer.h"

MessageDealer::MessageDealer() {
}

MessageDealer::MessageDealer(const MessageDealer& orig) {
}

MessageDealer::~MessageDealer() {
}

  void MessageDealer::showMessage(std::string message) {
 std::cout << message << std::endl; // show message on screen
}

 void MessageDealer::showErrorMessage(std::string error) {
 std::cerr << error << std::endl; // show error on screen
}