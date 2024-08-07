//
// Created by 宋博文 on 2024/8/4.
//

#include "../basic.h"

#define BUCKET_CAPACITY 100    // 桶的容量（单位：请求数）
#define LEAK_RATE (10 * 1000)           // 数据流出的速率（单位：每秒流出多少请求）

extern long long mstime();

typedef struct {
    int capacity;          // 桶的容量
    int excess;    // 当前桶中的请求量
    long long last;       // 上一次请求的时间戳
    pthread_mutex_t lock; // 互斥锁，用于线程安全
} LeakyBucket;


// 初始化漏桶
void bucket_init(LeakyBucket *bucket, int capacity) {
    bucket->capacity = capacity;
    bucket->excess = 0;
    bucket->last = 0;
    pthread_mutex_init(&bucket->lock, NULL);
}

// 销毁漏桶
void bucket_destroy(LeakyBucket *bucket) {
    pthread_mutex_destroy(&bucket->lock);
}

// 添加请求到桶
void add_request(LeakyBucket *bucket) {
    long long excess;
    pthread_mutex_lock(&bucket->lock);
    // 1. 计算距离上次请求过去多少时间
    long long now = mstime();
    long long elapsed_time = now - bucket->last;
    if (elapsed_time < 0) {
        elapsed_time = 0;
    }

    // 2. 计算漏出数量
    excess = bucket->excess - LEAK_RATE * elapsed_time / 1000 + 1000;
    if (excess < 0) {
        excess = 0;
    }
    if (excess > 0) {
        printf("Request overflow: %lld requests rejected.\n", excess);
        pthread_mutex_unlock(&bucket->lock);
        return;
    }
    bucket->excess = excess;
    bucket->last = now;
    printf("add Request: %lld requests passed.\n", excess);
    pthread_mutex_unlock(&bucket->lock);
}

TEST(leak_bucket_test, 测试漏桶限流) {
    LeakyBucket bucket;
    bucket_init(&bucket, BUCKET_CAPACITY);

    // 模拟请求流入
    usleep(10000);
    for (int i = 0; i < 20; i++) {
        add_request(&bucket);
        usleep(100000);
    }
    for (int i = 0; i < 5; i++) {
        add_request(&bucket);
    }
    bucket_destroy(&bucket);
}