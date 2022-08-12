//
// Created by Gavin on 2022/8/11.
//

#include "hello.h"

TEST(client, case1) {
    char message[30];
    int client_socket_fd;

    client_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {
        error_handling("socket() error");
    } else {
        success_handling("socket() success");
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(atoi("9999"));

    //调用 connect连接server
    if (connect(client_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    } else {
        success_handling("connect() success");
    }

    int str_len = read(client_socket_fd, message, sizeof(message) - 1);
    if (str_len == -1) {
        error_handling("read() error");
    } else {
        success_handling("read() success");
    }

    printf("Message from server : %s \n", message);
    // close连接
    close(client_socket_fd);
}

