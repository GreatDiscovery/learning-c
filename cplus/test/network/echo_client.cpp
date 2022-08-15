//
// Created by Gavin on 2022/8/15.
//

#include "hello.h"

TEST(echo_client, 回射客户端) {
    char message[BUFSIZ];
    int client_socket_fd;
    int str_len;

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

    while (1) {
        // fixme 标准输入和输出没有生效
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        write(client_socket_fd, message, strlen(message));
        str_len = read(client_socket_fd, message, BUF_SIZE - 1);
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    printf("quit! \n");
    // close连接
    close(client_socket_fd);
}