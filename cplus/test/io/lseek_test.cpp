//
// Created by Gavin on 2022/8/29.
//

#include "../basic.h"
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

// fixme 拷贝不成功
TEST(lseek_test, 拷贝文件) {
    int fd1, fd2;
    char buf[BUFSIZ];
    char ch;
    int str_len;

    //打开已存在文件和新建文件
    fd1 = open("a.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
    fd2 = open("b.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
    char message[] = "hello world";
    write(fd1, message, sizeof(message));
    std::cout << "before lseek position is " << lseek(fd1, 0, SEEK_CUR) << std::endl;
    // 重置到文件头的位置
    lseek(fd1, 0, SEEK_SET);
    std::cout << "after lseek position is " << lseek(fd1, 0, SEEK_CUR) << std::endl;
    while ((str_len = read(fd1, buf, BUFSIZ)) > 0) {
        std::cout << "buf:" << buf << std::endl;
        write(fd2, buf, str_len);
    }
    close(fd1);
    close(fd2);
}

