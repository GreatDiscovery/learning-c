#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "test/network/hello.h"
using namespace std;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

void error_handling(char *message) {
    std::cout << message << std::endl;
    exit(1);
}

void success_handling(char *message) {
    std::cout << message << std::endl;
}

int main() {
    char message[BUFSIZ];
    int client_socket_fd;
    int str_len;
    size_t line_cap;
    char *line = NULL;

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
    server_address.sin_port = htons(atoi("8888"));

    //调用 connect连接server
    if (connect(client_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    } else {
        success_handling("connect() success");
    }

    while (1) {
        fputs("Input message(Q to quit): ", stdout);
        std::cin.getline(message, BUFSIZ);
        if (!strcmp(message, "q") || !strcmp(message, "Q"))
            break;
        printf("Message from console: %s\n", message);
        write(client_socket_fd, message, strlen(message));
        str_len = read(client_socket_fd, message, BUF_SIZE - 1);
        message[str_len] = 0;
        printf("Message from server: %s\n", message);
    }
    printf("quit! \n");
    // close连接
    close(client_socket_fd);
}

