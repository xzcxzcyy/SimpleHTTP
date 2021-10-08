//
// Created by xzcxzcyy on 2021/10/7.
//

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include "socket_stream.h"

SocketStream::SocketStream(int fd) : sock_fd(fd) {}

std::string SocketStream::getline() {
    std::string line;

    while (true) {
        if (cursor >= end) {
            cursor = 0;
            end = recv(sock_fd, buffer, BufferSize, 0);
            if (end <= 0) {
                throw std::runtime_error("error recving from tcp socket.");
            }
            continue;
        }

        if (buffer[cursor] == '\n') {
            cursor++;
            return line;
        } else if (buffer[cursor] == '\r') {
            cursor++;
            continue;
        } else {
            line.push_back(buffer[cursor]);
            cursor++;
            continue;
        }
    }
}

void SocketStream::send(const std::string &msg) const {
    if (!msg.empty()) {
        write(sock_fd, msg.c_str(), msg.length());
    }

}

SocketStream::~SocketStream() {
    close(sock_fd);
}
