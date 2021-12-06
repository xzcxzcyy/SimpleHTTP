//
// Created by xzcxzcyy on 2021/10/7.
//

#ifndef SRC_CONFIG_H
#define SRC_CONFIG_H

#include <string>

/**
 * Global config of HTTP server.
 */
class Config {
public:
    Config() = default;

    explicit Config(std::string config_path);

    bool read_from_file();

    [[nodiscard]] const std::string &get_listen_addr() const;

    [[nodiscard]] int get_listen_port() const;

    [[nodiscard]] const std::string &get_main_dir() const;

    [[nodiscard]] const std::string &get_default_page() const;

private:
    std::string listen_addr;
    int listen_port{};
    std::string main_dir;
    std::string default_page;

private:
    std::string config_path;
};


#endif //SRC_CONFIG_H
