//
// Created by Gavin on 2023/3/11.
//

#include "vector_test.h"

using namespace std;

TEST(vector_test, vector迭代器) {
    vector<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    cout << "首个元素为：" << values.at(0) << endl;
    for (auto i = values.begin(); i < values.end(); i++) {
        cout << *i << " ";
    }
    cout << endl;
}

TEST(vector_test, vector去重) {
    // 示例 vector
    std::vector<int> vec = {1, 2, 3, 2, 4, 5, 3, 1, 6, 4};

    // 打印原始 vector
    std::cout << "Original vector: ";
    for (int num: vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 1. 对 vector 进行排序
    std::sort(vec.begin(), vec.end());

    // 2. 使用 std::unique 去除重复元素
    auto last = std::unique(vec.begin(), vec.end());

    // 3. 删除多余的元素
    vec.erase(last, vec.end());

    // 打印去重后的 vector
    std::cout << "Vector after removing duplicates: ";
    for (int num: vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}