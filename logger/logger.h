//
// Created by xzcxzcyy on 2021/10/22.
//

#ifndef SRC_LOGGER_H
#define SRC_LOGGER_H

#include <mutex>
#include <string>

/**
 * Thread-safe log.
 */
class Log {
private:
    std::mutex mu;

public:
    void i(const std::string &str);
};


#endif //SRC_LOGGER_H
