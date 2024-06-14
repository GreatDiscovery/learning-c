//
// Created by 贾芸 on 2024/6/14.
//

#include "../basic.h"

int sockpairfds[2];

void *thread_func(void *arg) {
    char buf[1];
    printf("Thread waiting for wakeup...\n");
    read(sockpairfds[1], buf, 1);
    printf("Thread woke up!\n");
    return NULL;
}

TEST(socketpair_test, 测试进程间通信唤醒另一个线程) {
    pthread_t thread;

    // 创建套接字对
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockpairfds) == -1) {
        perror("socketpair");
        exit(1);
    }

    // 创建线程并等待唤醒
    pthread_create(&thread, NULL, thread_func, NULL);

    // 模拟一些工作，然后唤醒线程
    sleep(1);
    write(sockpairfds[0], " ", 1);

    // 等待线程结束
    pthread_join(thread, NULL);

    // 关闭套接字对
    close(sockpairfds[0]);
    close(sockpairfds[1]);
}