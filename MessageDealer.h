/* 
 * File:   messageDealer.h
 * Author: renato
 *
 * Created on 30 de Agosto de 2016, 15:14
 */

#ifndef MESSAGEDEALER_H
#define MESSAGEDEALER_H

#include <iostream>     //std::cout, std::endl, std::cerr
#include <string>       //std::string

class MessageDealer {
public:
    MessageDealer();
    MessageDealer(const MessageDealer& orig);
    virtual ~MessageDealer();
    static void showMessage(std::string message);
    static void showErrorMessage(std::string error);
private:

};

#endif /* MESSAGEDEALER_H */

