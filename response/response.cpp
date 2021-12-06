//
// Created by xzcxzcyy on 2021/10/7.
//

#include "response.h"

#include <utility>

/**
 * Build a response.
 * @param version HTTP version.
 * @param status HTTP status.
 * @param content_type MIME type.
 */
Response::Response(std::string version, RespStatus status, std::string content_type)
        : version(std::move(version)), status(status), content_type(std::move(content_type)) {}

/**
 * Set content of the response.
 * @param str Content payload.
 * @return Reference to the response.
 */
Response & Response::set_content(std::string str) {
    content = std::move(str);
    return *this;
}

/**
 * String generator.
 */
std::string Response::to_string() {
    std::string ret;
    ret.append(version).append(" ").append(status_string.at(status));
    ret.append("\r\n");
    if (!content_type.empty()) {
        ret.append("Content-Type: ").append(content_type).append("\r\n");
    }
    ret.append("Content-Length: ").append(std::to_string(content.length())).append("\r\n");
    if (!location.empty()) {
        ret.append("Location: ").append(location).append("\r\n");
    }
    ret.append("\r\n").append(content);
    return ret;
}

Response &Response::set_location(std::string loc) {
    this->location = std::move(loc);
    return *this;
}
