//
// Created by 宋博文 on 2024/6/27.
//

#include "../basic.h"

TEST(pipe_test, 测试父子进程通信) {
    int pipefd[2];
    pid_t pid;
    char buf[30];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        close(pipefd[1]); // 关闭写端
        read(pipefd[0], buf, sizeof(buf));
        printf("Child received: %s\n", buf);
        close(pipefd[0]);
    } else {
        // 父进程
        close(pipefd[0]); // 关闭读端
        const char *msg = "Hello from parent";
        write(pipefd[1], msg, strlen(msg) + 1);
        close(pipefd[1]);
        wait(NULL);
    }
}