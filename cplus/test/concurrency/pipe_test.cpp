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

int pipefd[2];  // 管道文件描述符

void *writer_thread(void *arg) {
//    close(pipefd[0]);  // 关闭读端
    char message[] = "Hello from writer thread!";
    if (write(pipefd[1], message, strlen(message) + 1) == -1) {
        perror("write");
    } // 向管道写数据
    close(pipefd[1]);  // 写完后关闭写端
    return NULL;
}

void *reader_thread(void *arg) {
    char buf[1024];
//    close(pipefd[1]);  // 关闭写端
    if (read(pipefd[0], buf, sizeof(buf)) == -1) {
        perror("read");
    }  // 从管道读数据
    printf("Reader thread received: %s\n", buf);
    close(pipefd[0]);  // 读取完后关闭读端
    return NULL;
}

// 线程共享进程中的文件描述符
TEST(pipe_test, 测试线程间通信) {
    pthread_t writer, reader;

    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return ;
    }

    // 创建读线程和写线程
    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);
    // 等待线程完成
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
}