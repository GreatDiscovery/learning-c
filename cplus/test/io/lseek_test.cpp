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
    char ch;

    //打开已存在文件和新建文件
    fd1 = open("a.txt", O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, S_IRWXU);
    fd2 = open("b.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    char message[] = "hello world";
    write(fd1, message, sizeof(message));
    std::cout << "current position is " << lseek(fd1, 0, SEEK_CUR);
    // 重置到文件头的位置
    lseek(fd1, 0, SEEK_SET);

    lseek(fd1, -1, SEEK_END);//将文件位移量偏移到最后一个字符前
    read(fd1, &ch, 1);
    write(fd2, &ch, 1);

    //循环读取和写入
    while (lseek(fd1, -2, SEEK_CUR) != -1) {
        read(fd1, &ch, 1);
        write(fd2, &ch, 1);
    }

    close(fd1);
    close(fd2);
}

