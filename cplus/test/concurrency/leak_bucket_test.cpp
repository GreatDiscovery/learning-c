//
// Created by 宋博文 on 2024/8/4.
//

#include "../basic.h"

#define BUCKET_CAPACITY 100    // 桶的容量（单位：请求数）
#define LEAK_RATE 10           // 数据流出的速率（单位：每秒流出多少请求）

typedef struct {
    int capacity;          // 桶的容量
    int current_level;    // 当前桶中的请求量
//    long long last;       // 上一次请求的时间戳
    pthread_mutex_t lock; // 互斥锁，用于线程安全
} LeakyBucket;

// 初始化漏桶
void bucket_init(LeakyBucket *bucket, int capacity) {
    bucket->capacity = capacity;
    bucket->current_level = 0;
    pthread_mutex_init(&bucket->lock, NULL);
}

// 销毁漏桶
void bucket_destroy(LeakyBucket *bucket) {
    pthread_mutex_destroy(&bucket->lock);
}

// 流出线程
void *leak(void *arg) {
    LeakyBucket *bucket = (LeakyBucket *) arg;

    while (1) {
        sleep(1); // 每秒流出请求
        pthread_mutex_lock(&bucket->lock);
        if (bucket->current_level > 0) {
            bucket->current_level -= LEAK_RATE;
            if (bucket->current_level < 0) {
                bucket->current_level = 0;
            }
        }
        pthread_mutex_unlock(&bucket->lock);
    }

    return NULL;
}

// 添加请求到桶
// fixme
void add_request(LeakyBucket *bucket, int request) {
    pthread_mutex_lock(&bucket->lock);
    if (bucket->current_level + request > bucket->capacity) {
        printf("Request overflow: %d requests rejected.\n", bucket->current_level + request - bucket->capacity);
        bucket->current_level = bucket->capacity; // 桶满了，丢弃多余的请求
    } else {
        bucket->current_level += request;
        printf("Added %d requests. Current level: %d requests.\n", request, bucket->current_level);
    }
    pthread_mutex_unlock(&bucket->lock);
}

TEST(leak_bucket_test, 测试漏桶限流) {
    pthread_t leak_thread;
    LeakyBucket bucket;

    bucket_init(&bucket, BUCKET_CAPACITY);

    // 启动流出线程
    pthread_create(&leak_thread, NULL, leak, &bucket);

    // 模拟请求流入
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        int request = rand() % 30 + 1; // 生成 1 到 30 的随机请求数
        add_request(&bucket, request);
        sleep(2); // 模拟每两秒添加请求
    }

    pthread_cancel(leak_thread);
    pthread_join(leak_thread, NULL);
    bucket_destroy(&bucket);
}