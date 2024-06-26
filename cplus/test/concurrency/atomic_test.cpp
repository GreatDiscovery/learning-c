//
// Created by 贾芸 on 2023/9/14.
//

#include <thread>
#include <atomic>
#include "../basic.h"

// atomic operation
#define ngx_atomic_fetch_add(value, add)                                      \
    __sync_fetch_and_add(value, add)

#define ngx_atomic_cmp_set(lock, old, set)                                    \
    __sync_bool_compare_and_swap(lock, old, set)

std::atomic<int> atomicValue(0);

void incrementValue() {
    for (int i = 0; i < 1000; ++i) {
        ++atomicValue;
    }
}

TEST(atomic_test, 原子操作) {
    std::thread t1(incrementValue);
    std::thread t2(incrementValue);
    t1.join();
    t2.join();
    std::cout << "Final value: " << atomicValue << std::endl;
}

int a = 0;
void* incrementValue2(void *arg) {
    UNUSED(arg);
    for (int i = 0; i < 1000; i++) {
        ngx_atomic_fetch_add(&a, 1);
    }
    return NULL;
}


TEST(atomic_test, C语言的原子操作) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, incrementValue2, NULL);
    pthread_create(&t2, NULL, incrementValue2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("a=%d\n", a);
    assert(a == 2000);
}