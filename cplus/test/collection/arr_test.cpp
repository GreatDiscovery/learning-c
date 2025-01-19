//
// Created by 贾芸 on 2025/1/19.
//

#include "../basic.h"

struct person {
    int age;
    int height;
};

// 预先申请好了内存
static person personArr[5];

TEST(arr_test, C数组实现) {
    for (int i = 0; i < 5; i++) {
        personArr[i].age = i;
        personArr[i].height = i;
    }

    printf("age:%d\n", personArr[1].age);
    printf("height:%d\n", personArr[1].height);
}
