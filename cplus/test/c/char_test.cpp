//
// Created by 贾芸 on 2024/6/11.
//

#include "../basic.h"

void modifyString(char *str) {
    str[0] = 'X';
}

TEST(char_test, strdup新增一个副本防止修改原始字符串) {
    char original[] = "Hello, world!";
    char *copy = strdup(original);

    modifyString(copy);

    printf("Original: %s\n", original);  // 输出 "Hello, world!"，原始字符串未被修改
    printf("Copy: %s\n", copy);          // 输出 "Xello, world!"

    free(copy);  // 记得释放内存
}

char *getDynamicString() {
    char temp[] = "Temporary string";
    return strdup(temp);  // 返回动态分配的字符串副本
//    return temp; // temp的作用域只在该函数内
}

TEST(char_test, strdup动态内存管理) {
    char *dynamicString = getDynamicString();
    printf("Dynamic String: %s\n", dynamicString);

    free(dynamicString);  // 记得释放内存
}

TEST(char_test, 避免字符串常量段的问题) {
    char *str = "Hello, world!";
//    str[0] = 'X';  // 字符串常量通常存储在只读内存区域。试图修改这些字符串会导致未定义行为，通常是段错误（Segmentation Fault）
    // 使用 strdup 可以创建一个可修改的字符串副本
    char *copy = strdup(str);
    copy[0] = 'X';  // 合法操作

    printf("Copy: %s\n", copy);
}

TEST(char_test, 多个字符串拼接) {
    char str1[] = "Hello";
    char str2[] = ", ";
    char str3[] = "World!";
    char str4[] = " How are you?";

    char result[100];

    // 使用 sprintf 拼接字符串
    sprintf(result, "%s%s%s%s", str1, str2, str3, str4);

    printf("%s\n", result);
}

TEST(char_test, 修改单个字符) {
    char *str = strdup("oooohello world");
    uint32_t a = 1;
    char *p = str + 4;
    printf("%s\n", p);
    p[0] = 'w';
    printf("%s\n", p);
    p[0] = a + '0';
    printf("%s\n", p);
}