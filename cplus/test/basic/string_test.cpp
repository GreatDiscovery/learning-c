//
// Created by Gavin on 2022/8/16.
//

#include "../basic.h"
#include "string"
using namespace std;

TEST(rand_test, 测试随机数字) {
    cout << rand() << endl;
}

TEST(string_test, 字符串比较) {
    char *s1 = "hello";
    char *s2 = "world";
    char *s3 = "hello";
    cout << strcmp(s1, s2) << endl;
    cout << strcmp(s1, s3) << endl;
}