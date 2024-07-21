//
// Created by Gavin on 2022/8/16.
//

#include "../basic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


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
    char *info = "child";
    if (fork() == 0) {
        // 子进程
        sleep(1);
        printf("child=%s\n", info);
        assert(!strcmp(info, "child"));
        return;
    }
    // 父进程
    info = "parent";
    printf("parent=%s\n", info);
    assert(!strcmp(info, "parent"));
}

TEST(fork_test, copy_on_write2) {
    char *info = "child";
    if (fork() == 0) {
        // 子进程
        sleep(1);
        execl("/bin/ls", "ls", "-l", NULL);
        // execl执行了另一个程序来替代当前程序，所以下面内容不会打印出来
        printf("child=%s\n", info);
        return;
    }
    // 父进程
    info = "parent";
    printf("parent=%s\n", info);
    assert(!strcmp(info, "parent"));
}

TEST(fork_test, 测试拷贝页表耗时) {
    struct timeval start, end;
    int i;
    const int num_pages = 100000;
    const int page_size = 4096;

    // 分配内存
    char *memory = (char *) malloc(num_pages * page_size);
    if (memory == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 初始化内存
    for (i = 0; i < num_pages * page_size; i += page_size) {
        memory[i] = 'a';
    }

    // 获取fork开始时间
    gettimeofday(&start, NULL);

    // 创建子进程
    pid_t pid = fork();

    // 获取fork结束时间
    gettimeofday(&end, NULL);

    if (pid < 0) {
        perror("fork");
        free(memory);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("Child process: PID = %d\n", getpid());
        free(memory);  // 释放内存
        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        printf("Parent process: PID = %d\n", getpid());

        // 计算fork耗时
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        long elapsed = seconds * 1000000 + microseconds;

        // 大约24ms
        printf("Time taken for fork: %ld microseconds\n", elapsed);

        // 等待子进程结束
        wait(NULL);
        free(memory);  // 释放内存
    }
}

TEST(fork_test, malloc获取虚拟地址) {
    int *ptr = (int *)malloc(sizeof(int) * 10);  // 分配10个整数的内存
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    printf("Virtual address of allocated memory: %p\n", (void *)ptr);
    free(ptr);  // 释放分配的内存
}

TEST(fork_test, mmap获取虚拟地址) {
    int fd = open("example.txt", O_RDONLY|O_CREAT);
    if (fd == -1) {
        perror("open");
        return;
    }
    size_t length = 1024;
    void *addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    printf("Virtual address of mapped memory: %p\n", addr);
    munmap(addr, length);
    close(fd);
}