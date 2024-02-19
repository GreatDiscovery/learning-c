//
// Created by 贾芸 on 2024/2/19.
//

#include "../basic.h"

struct person {
    char name[8];
    int age;
};

TEST(mem_test, 测试memory函数) {
    struct person* arr;
    arr = static_cast<person *>(malloc(sizeof(struct person) * 10));
    printf("sizeof(struct person)=%d\n", sizeof(struct person));
    for (int i = 0; i < 10; ++i) {
        snprintf(arr[i].name, 10, "%d", i);
        arr[i].age = i;
    }
    printf("name=%s\n", arr[0].name);
    printf("age=%d\n", arr[0].age);
}