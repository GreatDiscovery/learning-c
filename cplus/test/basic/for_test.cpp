//
// Created by Gavin on 2022/7/31.
//

#include "gtest/gtest.h"
#include <iostream>
#include "vector"
#include <unistd.h>

int global_var = 10;

using namespace std;

void basic_for() {
    std::vector<int> arr = {1, 2, 3};
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        cout << *it << endl;
    }
}

void range_base_for() {
    std::vector<int> arr = {1, 2, 3};
    for (auto it : arr) {
        cout << it << endl;
    }
}

void modify_elements() {
    std::vector<int> arr = {1, 2, 3};
    for (auto &it : arr) {
        it = it * 2;
        cout << it << endl;
    }
    for (auto it : arr) {
        cout << it << endl;
    }
}

TEST(for_case, for1) {
    ASSERT_NO_FATAL_FAILURE(basic_for());
}

TEST(for_case, for2) {
    ASSERT_NO_FATAL_FAILURE(range_base_for());
}

TEST(for_case, for3) {
    ASSERT_NO_THROW(modify_elements());
}

typedef std::map<std::string, std::string> UserCollectedProperties;

TEST(for_case, for4) {
    char hostname[256]; // 声明一个数组来存储主机名
    if (gethostname(hostname, sizeof(hostname)) == 0) { // 获取主机名
        std::cout << "Hostname: " << hostname << std::endl; // 打印主机名
    } else {
        std::cerr << "Error: Unable to get hostname" << std::endl; // 打印错误信息
        return; // 返回错误码
    }
    for (const std::string& prefix : {"num.111", "num.333", "num.555"}) {
        cout << prefix << endl;
    }

    UserCollectedProperties props{{"num.111.1", "1"},
                                  {"num.111.2", "2"},
                                  {"num.111.3", "3"},
                                  {"num.333.1", "1"},
                                  {"num.333.2", "2"},
                                  {"num.333.3", "3"},
                                  {"num.555.1", "1"},
                                  {"num.555.2", "2"},
                                  {"num.555.3", "3"}, };
    for (const std::string& prefix :
            {"num.000", "num.222", "num.444", "num.666"}) {
        auto pos = props.lower_bound(prefix);
        ASSERT_TRUE(pos == props.end() ||
                    pos->first.compare(0, prefix.size(), prefix) != 0);
    }
}
