//
// Created by 贾芸 on 2024/3/18.
//

#include "../basic.h"
using namespace std;

TEST(map_test, 熟悉map的一些函数) {
    map<int, int> m;
    m[1] = 1;
    m[2] = 2;
    if (m.find(1) != m.end()) {
        cout << "1 exists in map" << endl;
    }
    if (m.find(3) != m.end()) {
        cout << "3 exists in map" << endl;
    }
}
