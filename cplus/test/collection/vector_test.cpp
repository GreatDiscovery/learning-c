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