//
// Created by Gavin on 2022/10/30.
//

#ifndef CPLUS_UNPIFI_H
#define CPLUS_UNPIFI_H

#include <locale>

#define IFI_NAME 16
#define IFI_HADDR 8

struct ifi_info {
    char ifi_name[IFI_NAME];
    short ifi_index;
    short ifi_mtu;
    u_char ifi_haddr[IFI_HADDR]; // hardware address
    u_short ifi_hlen; // hardware address bytes
    short ifi_flags;
    short ifi_myflags;
    struct sockaddr *ifi_primary_addr; // 主地址
    struct sockaddr *ifi_broadcast_addr; // 广播地址
    struct sockaddr *ifi_dest_addr; // 目标地址
    struct ifi_info *ifi_next; // 列表指针
};


struct ifi_info *get_ifi_info(int, int);

struct ifi_info *Get_ifi_info(int, int);

void free_ifi_info(struct ifi_info *);

#endif //CPLUS_UNPIFI_H
