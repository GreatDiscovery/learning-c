//
// Created by 宋博文 on 2024/8/4.
//

#include "../basic.h"

/* Return the UNIX time in microseconds */
long long ustime(void) {
    struct timeval tv;
    long long ust;

    gettimeofday(&tv, NULL);
    ust = ((long long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust;
}

TEST(time_test, 获取当前ms时间戳) {
    long long us = ustime();
    printf("current us=%lld\n", us);
}