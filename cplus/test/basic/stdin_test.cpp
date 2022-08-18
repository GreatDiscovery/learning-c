//
// Created by Gavin on 2022/8/18.
//

#include "../basic.h"
#include "fstream"
#include <fcntl.h>

using namespace std;

int readUserInput(std::istream &input) {
    int value;
    std::cout << "Enter a number: ";
    input >> value;
    return value;
}

TEST(stdin_test, 测试gtest标准输入) {
    std::ifstream ifs;
    int fd = open("input_file", O_CREAT | O_WRONLY, S_IRWXU);
    ifs.open("input_file", std::ifstream::in);
    // in production code pass std::cin
    // std::cout << "readUserInput from std::cin: " << readUserInput(std::cin) << std::endl;
    // in testing pass some mock data from the file (or whatever)
    std::cout << "readUserInput from ifs: " << readUserInput(ifs) << std::endl;
    std::cout << "flush" << flush;
    ifs.close();
}