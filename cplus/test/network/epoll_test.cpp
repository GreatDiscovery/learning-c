//
// Created by 贾芸 on 2025/1/10.
//

#include "../basic.h"
#ifndef __APPLE__
#include <sys/epoll.h>
#include <sys/eventfd.h>
#endif

int event_fd = -1;

void *read_thread(void *dummy) {
    uint64_t inc = 1;
    int ret = 0;
    int i = 0;
    for (; i < 10; i++) {
        ret = write(event_fd, &inc, sizeof(uint64_t));
        if (ret < 0) {
            perror("child thread write event_fd fail.");
        } else {
            printf("child thread completed write %llu (0x%llx) to event_fd\n", (unsigned long long) inc,
                   (unsigned long long) inc);
        }
        inc++;
    }
    return nullptr;
}

TEST(epoll_test, epolllevent使用) {
#ifndef __APPLE__
    int ret = 0;
    pthread_t pid = 0;

    event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    if (event_fd < 0) {
        perror("event_fd create fail.");
    }

    ret = pthread_create(&pid, NULL, read_thread, NULL);
    if (ret < 0) {
        perror("pthread create fail.");
    }
    sleep(2);

    uint64_t counter;
    int epoll_fd = -1;
    struct epoll_event events[16];

    if (event_fd < 0) {
        printf("event_fd not inited.\n");
    }

    epoll_fd = epoll_create(8);
    if (epoll_fd < 0) {
        perror("epoll_create fail:");
    }

    struct epoll_event read_event;
    read_event.events = EPOLLIN;
    read_event.data.fd = event_fd;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event_fd, &read_event);
    if (ret < 0) {
        perror("epoll_ctl failed:");
    }

    int epoll_once = 1;
    while (epoll_once) {
        printf("main thread epoll is waiting......\n");
        ret = epoll_wait(epoll_fd, events, 16, 2000);
        printf("main thread epoll_wait return ret : %d\n", ret);
        if (ret > 0) {
            int i = 0;
            for (; i < ret; i++) {
                int fd = events[i].data.fd;
                if (fd == event_fd) {
                    uint32_t epollEvents = events[i].events;
                    if (epollEvents & EPOLLIN) {
                        ret = read(event_fd, &counter, sizeof(uint64_t));
                        if (ret < 0) {
                            printf("main thread read fail\n");
                        } else {
                            printf("main thread read %llu (0x%llx) from event_fd\n", (unsigned long long) counter,
                                   (unsigned long long) counter);
                            epoll_once = 0;
                            break;
                        }
                    } else {
                        printf("main thread unexpected epoll events on event_fd\n");
                    }
                }
            }
        } else if (ret == 0) {
            printf("main thread epoll_wait timed out. continue epoll\n");
        } else {
            perror("main thread epoll_wait error.");
        }
    }
#endif
}
