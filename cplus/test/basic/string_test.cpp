//
// Created by Gavin on 2022/8/16.
//

#include <unordered_map>
#include <climits>
#include "../basic.h"
#include "string"

using namespace std;

void SplitString(const std::string &str, char delimiter, std::vector<std::string> &out);

TEST(rand_test, 测试字符串分割) {
    std::string s = "hello,world,hello,foo,bar";
    std::vector<std::string> result;

    // 分割字符串
    SplitString(s, ',', result);

    // 输出结果
    for (const auto &part: result) {
        std::cout << part << std::endl;
    }
}

void SplitString(const std::string &str, char delimiter, std::vector<std::string> &out) {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        out.push_back(item);
    }
}

struct server {
    long long int shard_id;
};

int string2ll(const char *s, size_t slen, long long *value);

void f1(long long int *l1);

void f1(long long int *l1) {
    *l1 = 5;
}


TEST(rand_test, 测试字符串转long) {
    struct server s{};
    s.shard_id = 1;
    long long int shard_id = s.shard_id;
    char str[] = "25";
    string2ll(str, sizeof(str) - 1, &shard_id);
    cout << "shard_id=" << shard_id << " s.shard_id=" << s.shard_id << endl;
    string2ll(str, sizeof(str) - 1, (&s.shard_id));
    cout << s.shard_id << endl;
    s.shard_id = 5;
    cout << s.shard_id << endl;
}


TEST(rand_test, 测试变量传递指针) {
    long long int a = 1;
    long long int *b = &a;
    f1(b);
    cout << "a=" << a << ",b=" << *b << endl;
}

int string2ll(const char *s, size_t slen, long long *value) {
    const char *p = s;
    size_t plen = 0;
    int negative = 0;
    unsigned long long v;

    if (plen == slen)
        return 0;

    /* Special case: first and only digit is 0. */
    if (slen == 1 && p[0] == '0') {
        if (value != NULL) *value = 0;
        return 1;
    }

    if (p[0] == '-') {
        negative = 1;
        p++;
        plen++;

        /* Abort on only a negative sign. */
        if (plen == slen)
            return 0;
    }

    /* First digit should be 1-9, otherwise the string should just be 0. */
    if (p[0] >= '1' && p[0] <= '9') {
        v = p[0] - '0';
        p++;
        plen++;
    } else if (p[0] == '0' && slen == 1) {
        *value = 0;
        return 1;
    } else {
        return 0;
    }

    while (plen < slen && p[0] >= '0' && p[0] <= '9') {
        if (v > (ULLONG_MAX / 10)) /* Overflow. */
            return 0;
        v *= 10;

        if (v > (ULLONG_MAX - (p[0] - '0'))) /* Overflow. */
            return 0;
        v += p[0] - '0';

        p++;
        plen++;
    }

    /* Return if not all bytes were used. */
    if (plen < slen)
        return 0;

    if (negative) {
        if (v > ((unsigned long long) (-(LLONG_MIN + 1)) + 1)) /* Overflow. */
            return 0;
        if (value != NULL) *value = -v;
    } else {
        if (v > LLONG_MAX) /* Overflow. */
            return 0;
        if (value != NULL) *value = v;
    }
    return 1;
}


TEST(rand_test, 测试随机数字冲突的概率) {
    // 统计相同值出现的次数
    std::unordered_map<int, int> countMap;

    // 生成随机数并统计
    for (int i = 0; i < 10000; ++i) {
        int randomNum = rand();
        ++countMap[randomNum];
    }

    // 输出相同值出现的次数
    for (const auto &pair: countMap) {
        if (pair.second > 1) {
            std::cout << "随机数 " << pair.first << " 出现了 " << pair.second << " 次" << std::endl;
        }
    }
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

TEST(string_test, resize用来追加或者缩减字符串) {
    using namespace std;
    string str1("Hello world");
    cout << "The original string str1 is: " << str1 << endl;

    basic_string<char>::size_type sizeStr1;
    sizeStr1 = str1.size();
    basic_string<char>::size_type capStr1;
    capStr1 = str1.capacity();

    // Compare size & capacity of the original string
    cout << "The current size of original string str1 is: "
            << sizeStr1 << "." << endl;
    cout << "The capacity of original string str1 is: "
            << capStr1 << "." << endl << endl;

    // Use resize to increase size by 2 elements: exclamations
    str1.resize(str1.size() + 2, '!');
    cout << "The resized string str1 is: " << str1 << endl;

    sizeStr1 = str1.size();
    capStr1 = str1.capacity();

    // Compare size & capacity of a string after resizing
    cout << "The current size of resized string str1 is: "
            << sizeStr1 << "." << endl;
    cout << "The capacity of resized string str1 is: "
            << capStr1 << "." << endl << endl;

    // Use resize to increase size by 20 elements:
    str1.resize(str1.size() + 20);
    cout << "The resized string str1 is: " << str1 << endl;

    sizeStr1 = str1.size();
    capStr1 = str1.capacity();

    // Compare size & capacity of a string after resizing
    // note capacity increases automatically as required
    cout << "The current size of modified string str1 is: "
            << sizeStr1 << "." << endl;
    cout << "The capacity of modified string str1 is: "
            << capStr1 << "." << endl << endl;

    // Use resize to downsize by 28 elements:
    str1.resize(str1.size() - 28);
    cout << "The downsized string str1 is: " << str1 << endl;

    sizeStr1 = str1.size();
    capStr1 = str1.capacity();

    // Compare size & capacity of a string after downsizing
    cout << "The current size of downsized string str1 is: "
            << sizeStr1 << "." << endl;
    cout << "The capacity of downsized string str1 is: "
            << capStr1 << "." << endl;
}

TEST(string_test, reserve缩小capacity保证足够容纳字符串) {
    using namespace std;
    string str1("Hello world");
    cout << "The original string str1 is: " << str1 << endl;

    basic_string<char>::size_type sizeStr1, sizerStr1;
    sizeStr1 = str1.size();
    basic_string<char>::size_type capStr1, caprStr1;
    capStr1 = str1.capacity();

    // Compare size & capacity of the original string
    cout << "The current size of original string str1 is: "
            << sizeStr1 << "." << endl;
    cout << "The capacity of original string str1 is: "
            << capStr1 << "." << endl << endl;

    // Compare size & capacity of the string
    // with added capacity
    str1.reserve(40);
    sizerStr1 = str1.size();
    caprStr1 = str1.capacity();

    cout << "The string str1with augmented capacity is: "
            << str1 << endl;
    cout << "The current size of string str1 is: "
            << sizerStr1 << "." << endl;
    cout << "The new capacity of string str1 is: "
            << caprStr1 << "." << endl << endl;

    // Compare size & capacity of the string
    // with downsized capacity
    str1.reserve();
    basic_string<char>::size_type sizedStr1;
    basic_string<char>::size_type capdStr1;
    sizedStr1 = str1.size();
    capdStr1 = str1.capacity();

    cout << "The string str1 with downsized capacity is: "
            << str1 << endl;
    cout << "The current size of string str1 is: "
            << sizedStr1 << "." << endl;
    cout << "The reduced capacity of string str1 is: "
            << capdStr1 << "." << endl << endl;
}
