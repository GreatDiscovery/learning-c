//
// Created by 宋博文 on 2024/1/29.
//

#include "../basic.h"

TEST(rand_1, rand测试) {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random floating-point number in the range [0, 1)
    double randomValue = (double) rand() / RAND_MAX;

    // Print the random value
    printf("Random Value: %f\n", randomValue);
    for (int i = 0; i < 100; i++) {
        randomValue = (double) rand() / RAND_MAX;
        printf("%f\n", randomValue);
    }
}