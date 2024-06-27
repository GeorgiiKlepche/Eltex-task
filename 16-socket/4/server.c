#include "functions.h"

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    prepare_sockaddr_in(&server_addr, "0.0.0.0", MAIN_PORT);

    sock_fd = create_socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(sock_fd, (struct sockaddr *)&server_addr, addr_len) == -1) {
        perror("Bind failed");
        close(sock_fd);
        return -1;
    }

    if (receive_packet(sock_fd, buffer, BUF_SIZE, (struct sockaddr *)&client_addr, &addr_len) == -1) {
        close(sock_fd);
        return -1;
    }

    printf("%s\n", buffer);
    buffer[0] = '\0';
    strcat(buffer, "Hello");

    if (send_packet(sock_fd, buffer, sizeof(buffer), (struct sockaddr *)&client_addr, addr_len) == -1) {
        close(sock_fd);
        return -1;
    }

    close(sock_fd);
    return 0;
}
