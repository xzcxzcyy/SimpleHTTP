//
// Created by xzcxzcyy on 2021/10/7.
//

#ifndef SRC_SOCKET_STREAM_H
#define SRC_SOCKET_STREAM_H

#include <string>

class SocketStream {
private:
    static const long BufferSize = 1024;

    const int sock_fd;
    char buffer[BufferSize]{};
    long end = 0; // exclusive ending
    long cursor = 0;

public:
    explicit SocketStream(int fd);

    std::string getline();

    void send(const std::string &msg) const;

    ~SocketStream();
};


#endif //SRC_SOCKET_STREAM_H
