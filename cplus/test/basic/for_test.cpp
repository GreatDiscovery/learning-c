//
// Created by Gavin on 2022/7/31.
//

#include "gtest/gtest.h"
#include <iostream>
#include "vector"

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