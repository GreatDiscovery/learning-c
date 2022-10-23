//
// Created by Gavin on 2022/10/23.
//

#include "../basic.h"

TEST(unix_local_test, local套接字) {
    char* path = "/tmp/s1";
    int sockfd;
    socklen_t len;
    struct sockaddr_un addr1, addr2;
    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(path);

    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, path, sizeof(addr1.sun_path) - 1);
    bind(sockfd, (sockaddr *) &addr1, SUN_LEN(&addr1));

    len = sizeof(addr2);
    getsockname(sockfd, (sockaddr *) &addr2, &len);
    printf("bound name = %s, return len = %d\n", addr2.sun_path, len);
}