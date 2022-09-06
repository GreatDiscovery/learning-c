//
// Created by Gavin on 2022/8/11.
//

#ifndef CPLUS_HELLO_H
#define CPLUS_HELLO_H

#include <iostream>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "string"
#define BUF_SIZE 1024
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#endif //CPLUS_HELLO_H

void error_handling(char *message);

void success_handling(char *message);

void str_echo(int sockfd);

void str_echo2(FILE *file, int sockfd);

void str_cli_select(FILE *fd, int sockfd);