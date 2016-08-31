/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

class messageDealer {
public:
    messageDealer();
    messageDealer(const messageDealer& orig);
    virtual ~messageDealer();
    static void showMessage(std::string message);
    static void showErrorMessage(std::string error);
private:

};

#endif /* MESSAGEDEALER_H */

