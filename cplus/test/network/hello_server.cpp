//
// Created by Gavin on 2022/8/9.
//

#include <iostream>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

void error_handling(char *message) {
    cout << message << endl;
    exit(1);
}

TEST(server, case1) {
    int server_socket_fd;
    int client_socket_fd;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        error_handling("socket() error");
    }
//    bind();
//    listen();
//    accept();

}

