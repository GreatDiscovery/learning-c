//
// Created by Gavin on 2023/2/5.
//

#include "../basic.h"
using namespace std;

TEST(std_test, 测试std所有的标准输入) {
    int a[5] = {1, 2, 3, 4, 5};
    // 采用的二分查找
    int *p = std::lower_bound(a, a + 5, 3);
    cout << "找到第一个不小于3的数， *p = " << *p << endl;

    int *q = std::upper_bound(a, a + 5, 3);
    cout << "第一个大于3的数, *q = " << *q << endl;
}


