#include "functions.h"

int main() {
    int sock_fd;
    struct ethernet_header eth_pkt;
    struct sockaddr_ll server_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_ll);
    char src_mac[] = {0x08, 0x8f, 0xc3, 0x37, 0xf7, 0x77};
    char dest_mac[] = {0x08, 0x8f, 0xc3, 0x37, 0xf7, 0x77};

    sock_fd = create_socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    prepare_ethernet_header(&eth_pkt, src_mac, dest_mac);

    server_addr.sll_family = AF_PACKET;
    server_addr.sll_protocol = htons(ETH_P_ALL);
    server_addr.sll_ifindex = if_nametoindex("enp4s0");
    server_addr.sll_hatype = 0;
    server_addr.sll_pkttype = 0;
    server_addr.sll_halen = 6;
    memcpy(server_addr.sll_addr, dest_mac, 6);

    prepare_ip_header(&eth_pkt.payload, "127.0.0.1", "127.0.0.1");
    prepare_udp_header(&eth_pkt.payload.payload, PORT, MAIN_PORT, "Hi");

    if (send_packet(sock_fd, &eth_pkt, BUF_SIZE, (struct sockaddr *)&server_addr, addr_len) == -1) {
        close(sock_fd);
        return -1;
    }

    while (1) {
        if (receive_packet(sock_fd, buffer, BUF_SIZE, (struct sockaddr *)&server_addr, &addr_len) == -1) {
            close(sock_fd);
            return -1;
        }

        struct ethernet_header *eth_pkt_recv = (struct ethernet_header *)buffer;
        printf("Пакет пойман");
        if (eth_pkt_recv->payload.payload.dest_port == eth_pkt.payload.payload.src_port) {
            printf(", server - %s\n", eth_pkt_recv->payload.payload.payload);
            break;
        } else {
            printf(" не от нашего сервера\n");
        }
    }

    close(sock_fd);
    return 0;
}
