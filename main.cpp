#include "response/response.h"
#include "config/config.h"
#include "io_util/socket_stream.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() {
    auto config = Config(string("./server_config.txt"));
    config.read_from_file();
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
    int new_fd = accept(listen_fd, (sockaddr *) &address, (socklen_t *) &addrlen);

    auto client_stream = SocketStream(new_fd);
    string req_line;
    while (!(req_line = client_stream.getline()).empty()) {
        client_stream.send(Response("HTTP/1.1", Ok, Text).set_content(req_line).to_string());
        cout << req_line << endl;
    }

    return 0;
}
