//
// Created by xzcxzcyy on 2021/10/22.
//

#include "logger.h"
#include <iostream>

void Log::i(const std::string &str) {
    std::unique_lock<std::mutex> lock(mu);
    std::cout << "[INFO] " << str
              << std::endl;
}
