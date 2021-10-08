//
// Created by xzcxzcyy on 2021/10/7.
//

#ifndef SRC_RESPONSE_H
#define SRC_RESPONSE_H

#include <string>
#include <unordered_map>

enum ContentType {
    Html,
    Jpeg,
};

enum RespStatus {
    Ok,
};

class Response {
private:
    const std::unordered_map<RespStatus, std::string> status_string{
            {Ok, "200 OK"},
    };
    const std::unordered_map<ContentType, std::string> type_string{
            {Html, "text/html"},
            {Jpeg, "image/jpeg"},
    };

    std::string version;
    RespStatus status;
    ContentType content_type;
    std::string content;

public:
    Response(std::string version, RespStatus status, ContentType content_type);

    Response & set_content(std::string str);

    std::string to_string();
};

#endif //SRC_RESPONSE_H
