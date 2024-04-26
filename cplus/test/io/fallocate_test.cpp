//
// Created by 贾芸 on 2024/4/22.
//

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>


TEST(fallocate_test, 测试顺序写) {
#ifndef __APPLE__
    const char *filename = "testfile.txt";
    int fd;
    off_t offset = 0;
    off_t len = 1024 * 1024; // 1MB

    // 打开文件
    fd = open(filename, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 分配空间
    if (fallocate(fd, 0, offset, len) == -1) {
        perror("fallocate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Space allocated successfully.\n");

    // 关闭文件
    close(fd);

    // 重新打开文件以便释放空间
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 释放空间
    if (fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE, offset, len) == -1) {
        perror("fallocate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Space deallocated successfully.\n");

    // 关闭文件
    close(fd);
#endif
}

uint32_t digits10(uint64_t v) {
    if (v < 10) return 1;
    if (v < 100) return 2;
    if (v < 1000) return 3;
    if (v < 1000000000000UL) {
        if (v < 100000000UL) {
            if (v < 1000000) {
                if (v < 10000) return 4;
                return 5 + (v >= 100000);
            }
            return 7 + (v >= 10000000UL);
        }
        if (v < 10000000000UL) {
            return 9 + (v >= 1000000000UL);
        }
        return 11 + (v >= 100000000000UL);
    }
    return 12 + digits10(v / 1000000000000UL);
}


int ll2string(char *dst, size_t dstlen, long long svalue) {
    static const char digits[201] =
            "0001020304050607080910111213141516171819"
            "2021222324252627282930313233343536373839"
            "4041424344454647484950515253545556575859"
            "6061626364656667686970717273747576777879"
            "8081828384858687888990919293949596979899";
    int negative;
    unsigned long long value;

    /* The main loop works with 64bit unsigned integers for simplicity, so
     * we convert the number here and remember if it is negative. */
    if (svalue < 0) {
        if (svalue != LLONG_MIN) {
            value = -svalue;
        } else {
            value = ((unsigned long long) LLONG_MAX) + 1;
        }
        negative = 1;
    } else {
        value = svalue;
        negative = 0;
    }

    /* Check length. */
    uint32_t const length = digits10(value) + negative;
    if (length >= dstlen) return 0;

    /* Null term. */
    uint32_t next = length;
    dst[next] = '\0';
    next--;
    while (value >= 100) {
        int const i = (value % 100) * 2;
        value /= 100;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
        next -= 2;
    }

    /* Handle last 1-2 digits. */
    if (value < 10) {
        dst[next] = '0' + (uint32_t) value;
    } else {
        int i = (uint32_t) value * 2;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
    }

    /* Add sign. */
    if (negative) dst[0] = '-';
    return length;
}


TEST(fallocate_test, 测试转换) {
    char buf[128];
    long long a = LLONG_MAX + LLONG_MAX - 1429844722;
    unsigned long long b = LLONG_MAX + 1;
    long long c = -1429844722;
    uint64_t d = -1429844722;
    ll2string(buf, sizeof(buf) - 1, d);
    printf("buf=%s\n", buf);
    printf("d=%lu\n", d);
}