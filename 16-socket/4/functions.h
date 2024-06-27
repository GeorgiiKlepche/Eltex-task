#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define IP_BUF_SIZE 33
#define MAIN_PORT 7780
#define PORT 7757

struct udp_header {
  short src_port;
  short dest_port;
  short length;
  short checksum;
  char payload[BUF_SIZE - 8];
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

struct ethernet_header {
  char dest_mac[6];
  char src_mac[6];
  short type;
  struct ip_header payload;
};

int create_socket(int domain, int type, int protocol);
void prepare_sockaddr_in(struct sockaddr_in *addr, const char *ip, int port);
void prepare_ethernet_header(struct ethernet_header *eth_hdr,
                             const char *src_mac, const char *dest_mac);
void prepare_ip_header(struct ip_header *ip_hdr, const char *src_ip,
                       const char *dest_ip);
void prepare_udp_header(struct udp_header *udp_hdr, int src_port, int dest_port,
                        const char *msg);
int send_packet(int fd, void *packet, int packet_size,
                struct sockaddr *dest_addr, socklen_t addrlen);
int receive_packet(int fd, void *buf, int buf_size, struct sockaddr *src_addr,
                   socklen_t *addrlen);
