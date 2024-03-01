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

TEST(string_test, 字符串内存拷贝) {

    // 下面使用的是字符串数组，属于是在堆里面开辟的一段数据，所以可以修改
    char s1[] = "hello, gavin!";
    char s2[] = "world";
    // 先把指针指向的内存清空，然后把数据填入
    memmove(s1, s2, 6);
    cout << "s1=" << s1 << endl;
    cout << "s2=" << s2 << endl;
    cout << "s2.len=" << sizeof(s2) << endl;

    // 这是两种定义字符串的方式，下面这种实际是字符串常量，存储在静态存储区，不能修改。
    const char *s3 = "hello!";
    const char *s4 = "world!";
    // error
//    memmove((void *) s3, (void *)s4, 7);
    cout << "s4=" << s4 << endl;

}