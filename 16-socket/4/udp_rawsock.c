#include "functions.h"

int main() {
    int sock_fd;
    struct udp_header udp_pkt;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE + 20];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    sock_fd = create_socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    prepare_sockaddr_in(&server_addr, "127.0.0.1", MAIN_PORT);

    prepare_udp_header(&udp_pkt, PORT, MAIN_PORT, "Hi");

    if (send_packet(sock_fd, &udp_pkt, BUF_SIZE, (struct sockaddr *)&server_addr, addr_len) == -1) {
        close(sock_fd);
        return -1;
    }

    while (1) {
        if (receive_packet(sock_fd, buffer, BUF_SIZE + 20, (struct sockaddr *)&server_addr, &addr_len) == -1) {
            close(sock_fd);
            return -1;
        }

        struct udp_header *udp_pkt_recv = (struct udp_header *)(buffer + 20);
        printf("Пакет пойман");
        if (udp_pkt_recv->dest_port == udp_pkt.src_port) {
            printf(", server - %s\n", udp_pkt_recv->payload);
            break;
        } else {
            printf(" не от нашего сервера\n");
        }
    }

    close(sock_fd);
    return 0;
}
