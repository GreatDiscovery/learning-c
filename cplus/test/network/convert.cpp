//
// Created by 贾芸 on 2023/12/4.
//

#include "convert.h"
#include "../basic.h"
#include "arpa/inet.h"

TEST(network_convert, ip转整数) {
    // ip转整数
    const char *ip_address = "192.168.0.1";
    unsigned int addr = inet_addr(ip_address);
    printf("IP address: %u\n", addr);

    // 整数转ip
    unsigned int addr2 = 3570666250;
    char buf[INET_ADDRSTRLEN];
    char *str = inet_neta(addr2, buf, sizeof(buf));
    printf("IP address: %s\n", str);
}