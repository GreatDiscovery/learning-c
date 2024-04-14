//
// Created by 贾芸 on 2024/3/30.
//

#include "../basic.h"

extern int global_var;

// 普通的全局变量不同，静态全局变量在程序的整个生命周期内都是可见的，但只能在声明它的源文件中访问。
static int global_variable = 10;

void foo() {
    static int x = 0;
    x++;
    printf("%d\n", x);
}
// static在函数调用结束后并不会被销毁，而是保留其值直到程序结束。这意味着静态变量在函数调用之间保持其值不变。这样声明，可以在非源文件也可以操作该值
TEST(keyword_test, static关键词测试) {
    foo();  // 输出 1
    foo();  // 输出 2
    foo();  // 输出 3
}

//如果你在一个文件中声明了一个全局变量或函数，但是该变量或函数的定义实际上是在另一个文件中，你可以使用extern关键字来声明这个变量或函数，以便在当前文件中使用它。
TEST(extern_test, extern关键词测试) {
    printf("global_var=%d\n", global_var);
    global_var++;
    printf("global_var=%d\n", global_var);
}