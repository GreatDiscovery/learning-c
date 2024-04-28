//
// Created by Gavin on 2022/8/16.
//

#include "../basic.h"
#include <unistd.h>
#include <fcntl.h>


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

TEST(fork_test, fork测试父子进程内存内容是否相同) {
    pid_t pid = fork(); // 创建子进程
    if (pid < 0) {
        // fork 出错处理
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("Child process executing...\n");
        // 在子进程中调用 exec 函数执行另一个程序
        execl("/bin/ls", "ls", "-l", NULL);
        // 如果 execl 函数返回，说明执行失败
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        printf("Parent process waiting for child...\n");
        // 等待子进程结束
        wait(NULL);
        printf("Parent process done.\n");
    }
}

/**
 * fork()之后，kernel把父进程中所有的内存页的权限都设为read-only，然后子进程的地址空间指向父进程。
 * 当父子进程都只读内存时，相安无事。当其中某个进程写内存时，CPU硬件检测到内存页是read-only的，于是触发页异常中断（page-fault），陷入kernel的一个中断例程。
 * 中断例程中，kernel就会 把触发的异常的页复制一份，于是父子进程各自持有独立的一份。
 */
 // this demo is so terrible
TEST(fork_test, copy_on_write) {
    int fd;
    char c[10];
    char *child = "#>Child.....output\n";

    fd = open("foobar.txt", O_RDWR | O_CREAT, 0666);
    printf("fd:%d\n", fd);
    write(fd, "foobar.txt", 7);
    close(fd);

    //父进程
    fd = open("foobar.txt", O_RDONLY, 0);
    fsync(fd);
    printf("fd:%d\n", fd);
    if (fork() == 0)//子进程
    {
        fd = 1;//stdout
        write(fd, child, strlen(child) + 1);
        fsync(fd);
        exit(0);
    }
    printf("fd:%d\n", fd);
    read(fd, c, sizeof(c));
    close(fd);
    c[10] = '\0';
    printf("c = %s\n", c);
    exit(0);
}