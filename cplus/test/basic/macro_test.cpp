//
// Created by Gavin on 2023/3/12.
// 宏测试，参考网址：https://zhuanlan.zhihu.com/p/447763456
//

#include "../basic.h"

using namespace std;

#define M 5
#define HELLO "hello \
the world"                  // 多行宏
#define MM M * M            // 宏的嵌套
#define COUNT(M) M * M               //定义有参宏

#define hello() do { \
    cout << "hello world!" << endl;                    \
} while(0)

TEST(macra_test, 宏测试) {
    cout << "M=" << M << endl;
    cout << "HELLO=" << HELLO << endl;
    cout << "MM=" << MM << endl;
    int x = 6;
    printf("COUNT = %d\n", COUNT(x + 1));// 输出结果： COUNT = 13
    printf("COUNT = %d\n", COUNT(++x));  // 输出结果： COUNT = 56
    hello();
}
