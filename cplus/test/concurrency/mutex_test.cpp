//
// Created by Gavin on 2023/3/20.
// unix系统编程，互斥量测试
//

#include "../basic.h"

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo *foo_alloc(int id) {
    struct foo *fp;
    if ((fp = static_cast<foo *>(malloc(sizeof(struct foo)))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, nullptr) != 0) {
            free(fp);
            return nullptr;
        }
    }
    return fp;
}