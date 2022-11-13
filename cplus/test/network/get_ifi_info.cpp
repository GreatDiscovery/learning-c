//
// Created by Gavin on 2022/10/30.
//

#include "../basic.h"
#include "unpifi.h"

TEST(get_ifi_info, 获取所有的up网络接口) {
    struct ifi_info *ifi, *ifi_head;
    struct sockaddr *sa;
    int family, doaliases, i;
    u_char *ptr;

    family = AF_INET;
    doaliases = std::stoi("eth0");

    for (ifi_head = ifi = Get_ifi_info(family, doaliases); ifi != NULL; ifi = ifi->ifi_next) {
        printf("%s: ", ifi->ifi_name);
        if (ifi->ifi_index != 0) {
            printf("(%d) ", ifi->ifi_index);
        }
        printf("<");
        if (ifi->ifi_flags & IFF_UP) printf("UP ");
        if (ifi->ifi_flags & IFF_BROADCAST) printf("BROADCAST ");
        if (ifi->ifi_flags & IFF_MULTICAST) printf("MULTICAST ");
        if (ifi->ifi_flags & IFF_LOOPBACK) printf("LOOPBACK ");
        if (ifi->ifi_flags & IFF_POINTOPOINT) printf("POINTOPOINT ");
        printf("> \n");

        if ((i = ifi->ifi_hlen) > 0) {
            ptr = ifi->ifi_haddr;
            do {
                // %x 16进制
                printf("%s%x", (i == ifi->ifi_hlen) ? "  " : ":", *ptr++);
            } while (--i > 0);
            printf("\n");
        }

        if (ifi->ifi_mtu != 0) {
            printf(" MTU: %d\n", ifi->ifi_mtu);
        }
        if ((sa = ifi->ifi_primary_addr) != NULL) {
            printf(" IP addr: %s\n", sa->sa_data);
        }
        if ((sa = ifi->ifi_broadcast_addr) != NULL) {
            printf(" broadcast addr: %s\n", sa->sa_data);
        }
        if ((sa = ifi->ifi_dest_addr) != NULL) {
            printf(" destination addr: %s\n", sa->sa_data);
        }
    }
}


/**
    网卡接口信息
    lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
    options=1203<RXCSUM,TXCSUM,TXSTATUS,SW_TIMESTAMP>
    inet 127.0.0.1 netmask 0xff000000
    inet6 ::1 prefixlen 128
    inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1
    nd6 options=201<PERFORMNUD,DAD>
    gif0: flags=8010<POINTOPOINT,MULTICAST> mtu 1280
    stf0: flags=0<> mtu 1280
    en5: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    ether ac:de:48:00:11:22
    inet6 fe80::aede:48ff:fe00:1122%en5 prefixlen 64 scopeid 0x4
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect (100baseTX <full-duplex>)
    status: active
 **/


struct ifi_info *get_ifi_info() {
    int sock_fd, last_len, len;
    struct ifreq *ifr;
    struct ifconf ifc;
    char *buf, lastname[BUFSIZ];
    struct ifi_info *ifi, *ifi_head, **ifi_p_next, *sdlname;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    last_len = 0;
    len = 100 * sizeof(struct ifreq);
    // 这里循环的目的是防止缓冲区不够，因为ioctl有可能会在返回截断的时候依旧返回成功
    for (;;) {
        buf = (char *) malloc(len);
        ifc.ifc_len = len;
        ifc.ifc_ifcu.ifcu_buf = buf;
        if (ioctl(sock_fd, SIOCGIFCONF, &ifc) < 0) {
        } else {
            if (ifc.ifc_len == last_len) {
                break;
            }
            last_len = ifc.ifc_len;
        }
        len += 10 * sizeof(struct ifreq);
        free(buf);
    }

    ifi_head = NULL;
    ifi_p_next = &ifi_head;
    lastname[0] = 0;
    sdlname = NULL;

}