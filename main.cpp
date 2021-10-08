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

using namespace std;

void session_handler(int fd);

Config config;

int main() {
    vector<thread> trds;

    config = Config(string("./server_config.txt"));
    bool config_success = config.read_from_file();
    if (!config_success) {
        cerr << "cannot read config" << endl;
        return -1;
    }
    int listen_fd;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "cannot create socket\n";
        return -1;
    }
    sockaddr_in address{};
    memset((char *) &address, 0, sizeof(address));

    address.sin_family = AF_INET;
    inet_pton(AF_INET, config.get_listen_addr().c_str(), &(address.sin_addr));
    address.sin_port = htons(config.get_listen_port());

    if (bind(listen_fd, (sockaddr *) &address, sizeof(address)) < 0) {
        cerr << "cannot bind\n";
        return -1;
    }

    if (listen(listen_fd, 3) < 0) {
        cerr << "cannot listen" << endl;
        return -1;
    }

    int addrlen = sizeof(address);
    while (true) {
        int new_fd = accept(listen_fd, (sockaddr *) &address, (socklen_t *) &addrlen);

        cout << "accepted successfully\n";

        if (new_fd < 0) {
            cerr << "cannot accept" << endl;
            return -1;
        }
        trds.emplace_back(session_handler, new_fd);
    }

    /*auto client_stream = SocketStream(new_fd);

    while (true) {
        auto [req_line, err] = client_stream.getline();
        if (err || req_line.empty()) {
            break;
        }
        client_stream.send(Response("HTTP/1.1", Ok, Text).set_content(req_line).to_string());
        cout << req_line << endl;
    }*/

}

void session_handler(int fd) {
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
                if (url == "/") {
                    url.append(config.get_default_page());
                }
                ifstream content_fin(config.get_main_dir() + url);

                string content_raw(std::istreambuf_iterator<char>{content_fin}, {});
                auto resp = Response(protocol_version, Ok, Html).set_content(content_raw);
                s_stream.send(resp.to_string());
            }
        }
    }
//    cout << "quitting\n";
}

