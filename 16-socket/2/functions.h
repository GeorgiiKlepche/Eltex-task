#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define MAIN_PORT 7780

struct udp_header {
    short src_port;
    short des_port;
    short length;
    short check_sum;
    char payload[BUF_SIZE - 42];
};

struct ip_header {
    char Ver_IHL;
    char DS;
    short length;
    short identification;
    short flags_offset;
    char TTL;
    char Transport_Proto;
    short Checksum;
    int src_ip;
    int dest_ip;
    struct udp_header payload;
};

struct ethernet_header { //14
    char des_mac[6];
    char src_mac[6];
    short type;
    struct ip_header payload;
};


