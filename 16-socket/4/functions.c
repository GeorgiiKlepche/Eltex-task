#include "functions.h"

int create_socket(int domain, int type, int protocol) {
  int fd = socket(domain, type, protocol);
  if (fd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }
  return fd;
}

void prepare_sockaddr_in(struct sockaddr_in *addr, const char *ip, int port) {
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(ip);
  addr->sin_port = htons(port);
}

void prepare_ethernet_header(struct ethernet_header *eth_hdr,
                             const char *src_mac, const char *dest_mac) {
  memcpy(eth_hdr->src_mac, src_mac, 6);
  memcpy(eth_hdr->dest_mac, dest_mac, 6);
  eth_hdr->type = htons(ETH_P_IP);
}

void prepare_ip_header(struct ip_header *ip_hdr, const char *src_ip,
                       const char *dest_ip) {
  ip_hdr->Ver_IHL = 69;
  ip_hdr->DS = 0;
  ip_hdr->length = htons(BUF_SIZE);
  ip_hdr->identification = 0;
  ip_hdr->flags_offset = 0;
  ip_hdr->TTL = 64;
  ip_hdr->Transport_Proto = 17;
  ip_hdr->Checksum = 0;
  ip_hdr->src_ip = inet_addr(src_ip);
  ip_hdr->dest_ip = inet_addr(dest_ip);
}

void prepare_udp_header(struct udp_header *udp_hdr, int src_port, int dest_port,
                        const char *msg) {
  udp_hdr->src_port = htons(src_port);
  udp_hdr->dest_port = htons(dest_port);
  udp_hdr->length = htons(BUF_SIZE - 20);
  udp_hdr->checksum = 0;
  strcpy(udp_hdr->payload, msg);
}

int send_packet(int fd, void *packet, int packet_size,
                struct sockaddr *dest_addr, socklen_t addrlen) {
  if (sendto(fd, packet, packet_size, 0, dest_addr, addrlen) == -1) {
    perror("Packet send failed");
    return -1;
  }
  return 0;
}

int receive_packet(int fd, void *buf, int buf_size, struct sockaddr *src_addr,
                   socklen_t *addrlen) {
  if (recvfrom(fd, buf, buf_size, 0, src_addr, addrlen) == -1) {
    perror("Packet receive failed");
    return -1;
  }
  return 0;
}
