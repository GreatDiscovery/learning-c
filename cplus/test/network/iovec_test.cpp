//
// Created by 贾芸 on 2025/1/3.
//

#include "../basic.h"
#include <fcntl.h>
#include <sys/uio.h>
#include <gtest/gtest.h>


TEST(iovec_test, 减少系统调用一次写入多条数据到socket中) {
    // 定义两个数据缓冲区
    char part1[] = "Hello, ";
    char part2[] = "World!\n";

    // 定义 iovec 数组
    struct iovec iov[2];
    iov[0].iov_base = part1; // 第一个缓冲区起始地址
    iov[0].iov_len = strlen(part1); // 第一个缓冲区长度
    iov[1].iov_base = part2; // 第二个缓冲区起始地址
    iov[1].iov_len = strlen(part2); // 第二个缓冲区长度

    // 将数据写到标准输出（文件描述符 1）
    ssize_t written = writev(1, iov, 2);
    if (written == -1) {
        perror("writev");
    }
}

TEST(iovec_test, 减少系统调用一次读取多条数据到socket中) {
    // 定义两个缓冲区
    char buf1[16];
    char buf2[16];

    // 定义 iovec 数组
    struct iovec iov[2];
    iov[0].iov_base = buf1; // 第一个缓冲区起始地址
    iov[0].iov_len = sizeof(buf1); // 第一个缓冲区长度
    iov[1].iov_base = buf2; // 第二个缓冲区起始地址
    iov[1].iov_len = sizeof(buf2); // 第二个缓冲区长度

    // 从标准输入读取数据
    ssize_t nread = readv(0, iov, 2);
    if (nread == -1) {
        perror("readv");
        return;
    }

    printf("Read %zd bytes:\n", nread);
    printf("Buffer 1: %.*s\n", (int) iov[0].iov_len, buf1);
    printf("Buffer 2: %.*s\n", (int) iov[1].iov_len, buf2);
}
