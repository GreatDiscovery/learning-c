//
// Created by Gavin on 2022/8/12.
// 测试shutdown，连接半关闭。要么关闭输入流，要么关闭输出流。比直接close更加灵活
//

#include "hello.h"
#define BUF_SIZE 30

TEST(shutdown_test, case1) {
    char buf[BUF_SIZE];
    int read_cnt;
    int server_socket_fd;
    int client_socket_fd;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        error_handling("socket() error");
    } else {
        success_handling("socket() success");
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi("9999"));
    //调用 bind 函数分配ip地址和端口号
    if (bind(server_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    } else {
        success_handling("bind() success");
    }
    //调用 listen 函数将套接字转为可接受连接状态
    if (listen(server_socket_fd, 10)) {
        error_handling("listen() error");
    } else {
        success_handling("listen() success");
    }
    // accept连接
    socklen_t client_addr_size = sizeof(client_address);
    client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_address, &client_addr_size);
    if (client_socket_fd == -1) {
        error_handling("accept() error");
    } else {
        success_handling("accept() success");
    }

    FILE *fp = fopen("shutdown_test.cpp", "rb");

    while (1) {
//        read_cnt = fread()
    }

    // close连接
    close(client_socket_fd);
    close(server_socket_fd);
}