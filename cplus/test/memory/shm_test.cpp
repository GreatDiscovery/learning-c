//
// Created by 宋博文 on 2024/6/29.
//

#include "../basic.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define BUFFER 4096
#define PERM S_IRUSR|S_IWUSR

TEST(shm_test, 新建共享内存) {
    // 创建一个新的共享内存段或者取得一个已.有共享内存段的标识符
    int shmid = shmget((key_t) 123456, BUFFER,
                       PERM | IPC_CREAT | IPC_EXCL);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    // 将共享内存段加载到调用进程的虚拟地址空间中
    char *shmaddr = (char *) shmat(shmid, NULL, 0);
    if (shmaddr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    // 从标准输入读数据
    // echo "hello world" > /proc/pid/fd/0
    fgets(shmaddr, 100, stdin);
    // 分离共享内存段
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }
    sleep(10); // 睡眠十秒后共享内存失效
    // 删除这块共享内存
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

TEST(shm_test, 获取共享内存) {
    int shmid = shmget((key_t) 123456, BUFFER, PERM);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    char *shmaddr = (char *) shmat(shmid, NULL, 0);
    if (shmaddr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    // 输出从另一个进程传过来的数据
    printf("%s\n", shmaddr);
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }
}