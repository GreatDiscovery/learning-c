//
// Created by 贾芸 on 2024/4/22.
//

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

TEST(fallocate_test, 测试顺序写) {
#ifndef __APPLE__
    const char *filename = "testfile.txt";
    int fd;
    off_t offset = 0;
    off_t len = 1024 * 1024; // 1MB

    // 打开文件
    fd = open(filename, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 分配空间
    if (fallocate(fd, 0, offset, len) == -1) {
        perror("fallocate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Space allocated successfully.\n");

    // 关闭文件
    close(fd);

    // 重新打开文件以便释放空间
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 释放空间
    if (fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE, offset, len) == -1) {
        perror("fallocate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Space deallocated successfully.\n");

    // 关闭文件
    close(fd);
#endif
}