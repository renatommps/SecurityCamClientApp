/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   messageDealer.cpp
 * Author: renato
 * 
 * Created on 30 de Agosto de 2016, 15:14
 */

#include "messageDealer.h"

messageDealer::messageDealer() {
}

messageDealer::messageDealer(const messageDealer& orig) {
}

messageDealer::~messageDealer() {
}

static void showMessage(std::string message) {
 std::cout << message << std::endl; // show message on screen
}

static void showErrorMessage(std::string error) {
 std::cerr << error << std::endl; // show error on screen
}