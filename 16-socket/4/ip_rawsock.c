#include "functions.h"

int main() {
  int sock_fd;
  struct ip_header ip_pkt;
  struct sockaddr_in server_addr, client_addr;
  char buffer[BUF_SIZE];
  socklen_t addr_len = sizeof(struct sockaddr_in);

  sock_fd = create_socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  prepare_sockaddr_in(&server_addr, "127.0.0.1", MAIN_PORT);

  int flag = 1;
  setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));

  prepare_ip_header(&ip_pkt, "127.0.0.1", "127.0.0.1");
  prepare_udp_header(&ip_pkt.payload, PORT, MAIN_PORT, "Hi");

  if (send_packet(sock_fd, &ip_pkt, BUF_SIZE, (struct sockaddr *)&server_addr,
                  addr_len) == -1) {
    close(sock_fd);
    return -1;
  }

  while (1) {
    if (receive_packet(sock_fd, buffer, BUF_SIZE,
                       (struct sockaddr *)&server_addr, &addr_len) == -1) {
      close(sock_fd);
      return -1;
    }

    struct ip_header *ip_pkt_recv = (struct ip_header *)buffer;
    printf("Пакет пойман");
    if (ip_pkt_recv->payload.dest_port == ip_pkt.payload.src_port) {
      printf(", server - %s\n", ip_pkt_recv->payload.payload);
      break;
    } else {
      printf(" не от нашего сервера\n");
    }
  }

  close(sock_fd);
  return 0;
}
