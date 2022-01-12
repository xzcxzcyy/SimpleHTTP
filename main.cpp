#include "response/response.h"
#include "config/config.h"
#include "io_util/socket_stream.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "logger/logger.h"
#include "mime/mime.h"


using namespace std;

void session_handler(int fd, const string &source_addr, int source_port, Log &logger);

Config config;

int main() {
    vector<thread> trds;
    Log logger;

    config = Config(string("./server_config.txt"));
    bool config_success = config.read_from_file();
    if (!config_success) {
        cerr << "cannot read config" << endl;
        return -1;
    }
    int listen_fd;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "cannot create socket" << endl;
        return -1;
    }

    int enable = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        cerr << "setsockopt error" << endl;
        return -1;
    }
    sockaddr_in address{};
    memset((char *) &address, 0, sizeof(address));

    address.sin_family = AF_INET;
    inet_pton(AF_INET, config.get_listen_addr().c_str(), &(address.sin_addr));
    address.sin_port = htons(config.get_listen_port());

    if (bind(listen_fd, (sockaddr *) &address, sizeof(address)) < 0) {
        cerr << "cannot bind" << endl;
        return -1;
    }

    if (listen(listen_fd, 3) < 0) {
        cerr << "cannot listen" << endl;
        return -1;
    }

    int addrlen = sizeof(address);
    while (true) {
        int new_fd = accept(listen_fd, (sockaddr *) &address, (socklen_t *) &addrlen);
        auto *pV4Addr = (sockaddr_in *) &address;
        in_addr ipAddr = pV4Addr->sin_addr;
        char addr_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, addr_str, INET_ADDRSTRLEN);
        auto port = pV4Addr->sin_port;

        cout << "accepted successfully\n";

        if (new_fd < 0) {
            cerr << "cannot accept" << endl;
            return -1;
        }

        trds.emplace_back(session_handler, new_fd, addr_str, port, std::ref(logger));
    }

}

void session_handler(int fd, const string &source_addr, int source_port, Log &logger) {

    auto s_stream = SocketStream(fd);
    while (true) {
        auto[req_line, err] = s_stream.getline();
        if (err) {
            break;
        }
        if (req_line.length() > 3) {
            stringstream ss(req_line);
            string method;
            ss >> method;
            if (method == "GET") {
                string url, protocol_version;
                ss >> url >> protocol_version;
                stringstream log_sstream;
                log_sstream << "From " << source_addr << ":" << source_port << " " << req_line << "\t";

                if (url == "/") {
                    url.append(config.get_default_page());
                }

                string fullPath = config.get_main_dir() + url;
                ifstream content_fin(fullPath);

                if (!content_fin.is_open()) {
                    auto resp = Response(protocol_version, NotFound, "");
                    s_stream.send(resp.to_string());
                    log_sstream << "Result 404";
                    logger.i(log_sstream.str());
                    continue;
                }

                string content_raw(std::istreambuf_iterator<char>{content_fin}, {});
                string file_mime_type = mime::lookup(fullPath);

                auto resp = Response(protocol_version, Ok, file_mime_type).set_content(content_raw);
                s_stream.send(resp.to_string());
                log_sstream << "Result 200";
                logger.i(log_sstream.str());
                //logger.i(resp.to_string());
            }
        }
    }
    logger.i("Thread quitting..");
}

