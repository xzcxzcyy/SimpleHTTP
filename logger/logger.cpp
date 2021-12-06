//
// Created by xzcxzcyy on 2021/10/22.
//

#include "logger.h"
#include <iostream>

/**
 * Send log of information level.
 * @param str Payload of this log.
 */
void Log::i(const std::string &str) {
    std::unique_lock<std::mutex> lock(mu);
    std::cout << "[INFO] " << str
              << std::endl;
}
