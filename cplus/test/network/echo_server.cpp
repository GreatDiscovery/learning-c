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

TEST(echo_server, 带select的server) {
    int listen_fd, connect_fd, maxi, maxfd, i, socket_fd, n;
    char buf[BUFSIZ];
    struct sockaddr_in server_addr, child_addr;
    int n_ready, client[FD_SETSIZE];
    fd_set read_set, all_set;
    socklen_t child_len;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // htonl等函数转换主机字节序和网络字节序：https://blog.csdn.net/bzhxuexi/article/details/20460655
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi("8888"));

    // C中结构体的强制转换的理解：https://codeantenna.com/a/9DKqkpLl8g
    bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(listen_fd, 10);
    success_handling("listen success!");

    maxfd = listen_fd;
    maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);

    while (true) {
        // 这里有一个拷贝赋值
        read_set = all_set;
        n_ready = select(maxfd + 1, &read_set, NULL, NULL, NULL);
        // 有新链接进来
        if (FD_ISSET(listen_fd, &read_set)) {
            child_len = sizeof(child_addr);
            // accept的第三个参数是一个典型的值-结果参数。进程传值给内核，内核利用该指针返回结果。
            connect_fd = accept(listen_fd, (struct sockaddr *) &child_addr, &child_len);
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    // server端自己维护一套fd
                    client[i] = connect_fd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                error_handling("too many clients");
            }
            FD_SET(connect_fd, &all_set);
            if (connect_fd > maxfd) {
                maxfd = connect_fd;
            }
            if (i > maxi) {
                // 记录client里记录的最大的fd的索引，避免整个client循环
                maxi = i;
            }
            if (--n_ready <= 0) {
                continue;
            }
        }

        // 循环所有已经建立连接的fd，判断是否read for read
        for (i = 0; i <= maxi; i++) {
            if ((socket_fd = client[i]) < 0) {
                continue;
            }
            if (FD_ISSET(socket_fd, &read_set)) {
                if ((n = read(socket_fd, buf, BUFSIZ)) == 0) {
                    // 没有读到数据，说明客户端断开了连接
                    close(socket_fd);
                    FD_CLR(socket_fd, &all_set);
                    client[i] = -1;
                } else {
                    write(socket_fd, buf, n);
                }
                if (--n_ready <= 0) {
                    break;
                }
            }
        }

    }

}

TEST(echo_client, server进程正常终止client收到RST) {
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

    int fd = open("data.txt", O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
    if (fd == -1) {
        error_handling("open() failed!");
    }

    FILE *file = fopen("data.txt", "r");

    while (1) {
        str_echo2(file, client_socket_fd);
        sleep(5);
    }
    std::cout << "quit!" << std::endl;
    close(fd);
    // close连接
    close(client_socket_fd);
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

    int fd = open("data.txt", O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
    if (fd == -1) {
        error_handling("open() failed!");
    }
    while (1) {
        std::cout << "Input message from data.txt (Q to quit):" << std::endl;
        str_len = read(fd, message, sizeof(message));
        if (str_len > 0) {
            message[str_len] = 0;
        }
        lseek(fd, 0, SEEK_SET);
        std::cout << "current position:" << lseek(fd, 0, SEEK_CUR) << std::endl;

        if (!strcmp(message, "q") || !strcmp(message, "Q"))
            break;
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

TEST(echo_client, select防止阻塞在某个fd上) {
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
    server_address.sin_port = htons(atoi("8888"));

    //调用 connect连接server
    if (connect(client_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    } else {
        success_handling("connect() success");
    }

    int fd = open("data.txt", O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
    if (fd == -1) {
        error_handling("open() failed!");
    }

    FILE *file = fopen("data.txt", "r");


    str_cli_select(file, client_socket_fd);

    std::cout << "quit!" << std::endl;
    close(fd);
    // close连接
    close(client_socket_fd);
}

TEST(echo_client, 调用shutdown单方向关闭) {
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
    server_address.sin_port = htons(atoi("8888"));

    //调用 connect连接server
    if (connect(client_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    } else {
        success_handling("connect() success");
    }

    int fd = open("data.txt", O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
    if (fd == -1) {
        error_handling("open() failed!");
    }

    FILE *file = fopen("data.txt", "r");

    str_cli_shutdown(file, client_socket_fd);

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
    }
}

void str_echo2(FILE *fp, int connect_fd) {
    char send_line[BUFSIZ], receive_line[BUFSIZ];
    while (fgets(send_line, BUFSIZ, fp) != NULL) {
        write(connect_fd, send_line, strlen(send_line));
        read(connect_fd, receive_line, BUFSIZ);
        std::cout << "receive message:" << receive_line << std::endl;
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

// select主要解决客户端单阻塞问题
void str_cli_select(FILE *fp, int sockfd) {
    int maxfdp1;
    fd_set rset;
    char send_line[BUFSIZ], receive_line[BUFSIZ];

    FD_ZERO(&rset);
    while (true) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = std::max(fileno(fp), sockfd) + 1;
        select(maxfdp1, &rset, nullptr, nullptr, nullptr);

        // fd is ready.
        if (FD_ISSET(sockfd, &rset)) {
            read(sockfd, receive_line, BUFSIZ);
            std::cout << "receive from server:" << receive_line << std::endl;
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if (fgets(send_line, BUFSIZ, fp) == nullptr) {
                return;
            }
            std::cout << "receive from file:" << send_line << std::endl;
            write(sockfd, send_line, strlen(send_line));
        }
        // todo 必须睡眠一段时间，否则select fd没有准备好。测试该事件如何配置
        sleep(5);

    }
}

void str_cli_shutdown(FILE *fp, int sockfd) {
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[BUFSIZ];
    int len;

    stdineof = 0;
    FD_ZERO(&rset);
    while (true) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = std::max(fileno(fp), sockfd) + 1;
        int n_ready = select(maxfdp1, &rset, nullptr, nullptr, nullptr);

        // fd is ready.
        if (FD_ISSET(sockfd, &rset)) {
            if ((len = read(sockfd, buf, BUFSIZ)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    error_handling("str_cli: server terminated prematurely");
                }
            }
            std::cout << "receive from server:" << buf << std::endl;
            write(fileno(stdout), buf, len);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if ((len = read(fileno(fp), buf, BUFSIZ)) == 0) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            std::cout << "receive from file:" << buf << std::endl;
            write(sockfd, buf, len);
        }
    }
}