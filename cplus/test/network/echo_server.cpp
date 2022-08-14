//
// Created by Gavin on 2022/8/14.
//

// 回射函数，客户端输入什么，服务端返回什么
#include "hello.h"

TEST(echo_test, 回射server) {
    int server_listen_fd;
    int client_socket_fd;
    int server_connect_fd;

    pid_t child_pid;
    socklen_t child_len;

    struct sockaddr_in server_address;
    struct sockaddr_in child_server_address;
    struct sockaddr_in client_address;
    server_listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_listen_fd == -1) {
        error_handling("socket() error");
    } else {
        success_handling("socket() success");
    }

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi("9999"));
    //调用 bind 函数分配ip地址和端口号
    if (bind(server_listen_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    } else {
        success_handling("bind() success");
    }
    //调用 listen 函数将套接字转为可接受连接状态
    if (listen(server_listen_fd, 10)) {
        error_handling("listen() error");
    } else {
        success_handling("listen() success");
    }
    // accept连接
    socklen_t client_addr_size = sizeof(client_address);
    client_socket_fd = accept(server_listen_fd, (struct sockaddr *) &client_address, &client_addr_size);
    if (client_socket_fd == -1) {
        error_handling("accept() error");
    } else {
        success_handling("accept() success");
    }


    while (1) {
        child_len = sizeof(child_pid);
        server_connect_fd = accept(server_listen_fd, (struct sockaddr *) &child_server_address, &child_len);
        if ((child_pid = fork()) == 0) {
            close(server_listen_fd);
            str_echo(server_connect_fd);
            exit(0);
        }
        close(server_connect_fd);
    }

    // close连接
    close(client_socket_fd);
    close(server_listen_fd);
}