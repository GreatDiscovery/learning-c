//
// Created by Gavin on 2022/8/14.
//

// 回射函数，客户端输入什么，服务端返回什么
#include "hello.h"
#include "../io/file_test.h"
#include <unistd.h>

void sig_child(int signo);

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
    server_address.sin_port = htons(atoi("8888"));
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

    // 信号处理
    struct sigaction act, oldact;
    act.sa_handler = sig_child;
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = SA_RESETHAND | SA_NODEFER;
    sigaction(SIGCHLD, &act, &oldact);
    sigaction(SIGTERM, &act, &oldact);

//    sleep(600);

    while (1) {
        child_len = sizeof(child_pid);
        server_connect_fd = accept(server_listen_fd, (struct sockaddr *) &child_server_address, &child_len);
        if (server_connect_fd != -1) {
            success_handling("accept() success");
        }
        if ((child_pid = fork()) == 0) {
            std::cout << "child_pid=" << child_pid << std::endl;
            std::cout << "current_pid=" << getpid() << std::endl;
            close(server_listen_fd);
            str_echo(server_connect_fd);
            exit(0);
        }
        close(server_connect_fd);
    }
}

TEST(echo_client, server进程正常终止) {
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

    int fd = open("data.txt", O_CREAT, RWRWRW);
    if (fd == -1) {
        error_handling("open() failed!");
    }
    while (1) {
        std::cout << "Input message from data.txt (Q to quit):" << std::endl;
        if (read(fd, message, sizeof(message)) == -1) {
            error_handling("read() failed!");
        }
        if (!strcmp(message, "q") || !strcmp(message, "Q"))
            break;
        // fixme file read position problem
        std::cout << "Message from console:" << message << std::endl;
        write(client_socket_fd, message, strlen(message));
        str_len = read(client_socket_fd, message, BUF_SIZE - 1);
        message[str_len] = 0;
        std::cout << "Message from server:" << message << std::endl;
        sleep(5);
    }
    std::cout << "quit!" << std::endl;
    close(fd);
    // close连接
    close(client_socket_fd);
}

void str_echo(int conn_fd) {
    int str_len;
    char message[BUFSIZ];
    while ((str_len = read(conn_fd, message, BUFSIZ)) > 0) {
        std::cout << "Message from client:" << message << std::endl;
        write(conn_fd, message, str_len);
        sleep(1);
    }
}

// signal handler
void sig_child(int signo) {
    pid_t pid;
    int stat;
//    pid = wait(&stat);
    // 处理所有终止的子进程。wait会阻塞，而wati_pid加了WNOHANG不会阻塞
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        std::cout << "child " << pid << " terminated" << std::endl;
    }
}