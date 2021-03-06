//
// Created by xzcxzcyy on 2021/10/7.
//

#ifndef SRC_RESPONSE_H
#define SRC_RESPONSE_H

#include <string>
#include <unordered_map>

enum RespStatus {
    Ok,
    NotFound,
    Moved,
};

/**
 * HTTP response.
 */
class Response {
private:
    const std::unordered_map<RespStatus, std::string> status_string{
            {Ok, "200 OK"},
            {NotFound, "404 Not Found"},
            {Moved, "302 Found"}
    };

    std::string version;
    RespStatus status;
    std::string content_type;
    std::string content;
    std::string location;

public:
    Response(std::string version, RespStatus status, std::string content_type);

    Response & set_content(std::string str);

    Response & set_location(std::string loc);

    std::string to_string();
};

#endif //SRC_RESPONSE_H
