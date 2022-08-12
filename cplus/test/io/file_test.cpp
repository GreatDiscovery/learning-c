//
// Created by Gavin on 2022/8/12.
// 文件读取测试
//

#include "file_test.h"
#include "../network/hello.h"

#define BUF_SIZE 100

TEST(file_test, write_test) {
    char buf[] = "hello world!\n";
    // 创建 | 只写 | 如果有内容清空
    // 会在对应的test目录下创建新的文件，这里创建的文件没有相关的权限，最后一个参数赋予了权限
    int fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd == -1) {
        error_handling("open() failed!");
    }
    if (write(fd, buf, sizeof(buf)) == -1) {
        error_handling("write() failed!");
    }
    close(fd);
}


TEST(file_test, read_test) {
    char buf[BUF_SIZE];
    int fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        error_handling("open() failed!");
    }
    if (read(fd, buf, sizeof(buf)) == -1) {
        error_handling("read() failed!");
    }
    printf("file data: %s", buf);
    close(fd);
}

