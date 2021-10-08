//
// Created by xzcxzcyy on 2021/10/7.
//

#include "response.h"

Response::Response(std::string version, RespStatus status, ContentType content_type)
        : version(std::move(version)), status(status), content_type(content_type) {}

Response & Response::set_content(std::string str) {
    content = std::move(str);
    return *this;
}

std::string Response::to_string() {
    std::string ret;
    ret.append(version).append(" ").append(status_string.at(status));
    ret.append("\r\n");
    ret.append(type_string.at(content_type));
    ret.append("Content-Length: ").append(std::to_string(content.length())).append("\r\n");
    ret.append("\r\n").append(content);
    return ret;
}
