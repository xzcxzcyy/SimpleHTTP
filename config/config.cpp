//
// Created by xzcxzcyy on 2021/10/7.
//

#include "config.h"
#include <fstream>
#include <string>
#include <utility>

/**
 * Constructor of Config.
 * @param config_path Path to config file.
 */
Config::Config(std::string config_path) : config_path(std::move(config_path)) {}

/**
 * Read config from file.
 * @return Whether it is a successful read.
 */
bool Config::read_from_file() {
    std::ifstream fin = std::ifstream(config_path);
    if (!fin.is_open()) {
        return false;
    }
    fin >> listen_addr >> listen_port >> main_dir >> default_page;
    return !listen_addr.empty() && (listen_port != 0) && !main_dir.empty() && !default_page.empty();
}

const std::string &Config::get_listen_addr() const {
    return listen_addr;
}

int Config::get_listen_port() const {
    return listen_port;
}

const std::string &Config::get_main_dir() const {
    return main_dir;
}

const std::string &Config::get_default_page() const {
    return default_page;
}
