//
// Created by Gavin on 2022/8/16.
//

#include "../basic.h"
#include <unistd.h>


TEST(fort_test, fork测试) {

    pid_t pid;
    //判断1
    if ((pid = fork()) < 0) {
        perror("fork error");
    }
        //判断2
    else if (pid == 0)//子进程
    {
        printf("child getpid()=%d\n", getpid());
    }
        //判断3
    else if (pid > 0)//父进程
    {
        printf("parent getpid()=%d\n", getpid());
    }
}