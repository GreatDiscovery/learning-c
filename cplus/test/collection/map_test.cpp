//
// Created by 贾芸 on 2024/3/18.
//

#include "../basic.h"
#include <unordered_map>
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


TEST(map_test, unorderd_map测试) {
    // 创建一个 unordered_map，将字符串映射到整数
    std::unordered_map<std::string, int> umap;

    // 插入一些键值对
    umap["apple"] = 10;
    umap["banana"] = 20;
    umap["cherry"] = 30;

    // 访问键值对
    std::cout << "apple: " << umap["apple"] << std::endl;
    std::cout << "banana: " << umap["banana"] << std::endl;
    std::cout << "cherry: " << umap["cherry"] << std::endl;

    // 检查某个键是否存在
    if (umap.find("banana") != umap.end()) {
        std::cout << "banana found" << std::endl;
    }

    // 删除某个键值对
    umap.erase("banana");

    // 检查删除后的键是否存在
    if (umap.find("banana") == umap.end()) {
        std::cout << "banana not found" << std::endl;
    }
}
