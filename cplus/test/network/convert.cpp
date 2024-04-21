//
// Created by 贾芸 on 2023/12/4.
//

#include "../basic.h"
#include "arpa/inet.h"
#ifndef __APPLE_
#endif

TEST(network_convert, ip转整数) {
    // ip转整数
    const char *ip_address = "192.168.0.1";
    unsigned int addr = inet_addr(ip_address);
    printf("IP address: %u\n", addr);

    // 整数转ip
    unsigned int addr2 = 3570666250;
    char buf[INET_ADDRSTRLEN];
#ifdef __APPLE__
    char *str = inet_neta(addr2, buf, sizeof(buf));
#else
    const char *str = inet_ntop(AF_INET, &addr2, buf, sizeof(buf));
    printf("IP address: %s\n", buf);
#endif
    printf("IP address: %s\n", str);
}