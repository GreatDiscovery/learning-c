//
// Created by 贾芸 on 2024/6/13.
//

#include "../basic.h"
#include <stdarg.h>

// 可变参数函数示例
void print_numbers(const char *label, int num, ...) {
    va_list args;                  // 定义一个 va_list 类型的变量
    va_start(args, num);           // 初始化 va_list 变量

    printf("%s: ", label);         // 打印标签
    for (int i = 0; i < num; ++i) {
        int value = va_arg(args, int); // 获取下一个参数
        printf("%d ", value);          // 打印参数
    }
    va_end(args);                  // 结束可变参数列表的处理
    printf("\n");
}

TEST(va_test, 测试可变参数函数) {
    print_numbers("Numbers", 3, 10, 20, 30);
}
