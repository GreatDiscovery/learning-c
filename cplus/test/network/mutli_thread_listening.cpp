//
// Created by 宋博文 on 2023/12/29.
//

#include "hello.h"

// The SO_REUSEPORT option allows multiple sockets to bind to the same port simultaneously.
static int reuse_port(int fd) {
    int optval = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int)) == -1) {
        return -1;
    }
    return 1;
}

TEST(server, 同一个port绑定多个fd) {
    int server_socket_fd, second_server_socket_fd;
    int client_socket_fd;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    second_server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1 || second_server_socket_fd == -1) {
        error_handling("socket() error");
    } else {
        success_handling("socket() success");
    }
    reuse_port(server_socket_fd);
    reuse_port(second_server_socket_fd);

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

    // 第二次绑定
    if (bind(second_server_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
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

    // 第二次listen
    if (listen(second_server_socket_fd, 10)) {
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

    char message[] = "hello socket";
    write(client_socket_fd, message, sizeof(message));

    // close连接
    close(client_socket_fd);
    close(server_socket_fd);
}

