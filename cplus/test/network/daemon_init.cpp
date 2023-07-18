//
// Created by Gavin on 2022/10/15.
// 进程转为守护进程
//
#include "syslog.h"
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#define MAXFD 64

int daemon_init(const char *pname, int facility) {
    int i;
    pid_t pid;

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);
    }

    if (setsid() < 0) {
        return -1;
    }

    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);
    }
    // child2

    int daemon_proc = 1;
    chdir("/");

    for (int i = 0; i < MAXFD; i++) {
        close(i);
    }

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pname, LOG_PID, facility);
    return 0;

}