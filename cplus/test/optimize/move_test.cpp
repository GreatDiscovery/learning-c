//
// Created by 贾芸 on 2024/4/29.
//

#include <gtest/gtest.h>

struct MyData {
    std::string data;

    MyData(const char *d) : data(d) {}

    MyData(MyData &&other) : data(std::move(other.data)) {
        std::cout << "Move constructor called!" << std::endl;
    }
};

// https://blog.csdn.net/qq_39560854/article/details/137465481?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-137465481-blog-112754585.235^v43^pc_blog_bottom_relevance_base7&spm=1001.2101.3001.4242.1&utm_relevant_index=3
TEST(move_test, move测试) {
    std::vector<MyData> vec;
    vec.push_back(MyData("Hello"));
    std::vector<MyData> vec2 = std::move(vec);

    std::cout << "vec size: " << vec.size() << std::endl;// vec现在为空
    std::cout << "vec2 size: " << vec2.size() << std::endl;// vec2拥有之前vec的内容

    std::unique_ptr<int> ptr1 = std::make_unique<int>(10);
    std::unique_ptr<int> ptr2 = std::move(ptr1);

    std::cout << "ptr1: " << (ptr1 ? "not null" : "null") << std::endl;// ptr1现在为空
    std::cout << "ptr2: " << (ptr2 ? "not null" : "null") << std::endl;// ptr2拥有之前ptr1的内容
}