#include "functions.h"

int main() {
  int tcp_fd, udp_fd;
  struct sockaddr_in server_addr;
  char buf[BUF_SIZE];
  ssize_t bytes_read, bytes_sent;
  socklen_t client_len;
  struct sockaddr_in client_addr;

  tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_fd == -1) {
    perror("Error creating TCP socket");
    return EXIT_FAILURE;
  }

  udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_fd == -1) {
    perror("Error creating UDP socket");
    close(tcp_fd);
    return EXIT_FAILURE;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(MAIN_PORT);

  if (bind(tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Error binding TCP socket");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  if (bind(udp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Error binding UDP socket");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  printf("Server listening on port %d (TCP/UDP)...\n", MAIN_PORT);

  while (1) {
    if (listen(tcp_fd, 5) == -1) {
      perror("Error listening on TCP socket");
      continue;
    }

    client_len = sizeof(client_addr);
    int client_fd =
        accept(tcp_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
      perror("Error accepting TCP connection");
      continue;
    }

    printf("TCP Connection accepted from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    bytes_read = recv(client_fd, buf, sizeof(buf), 0);
    if (bytes_read == -1) {
      perror("Error reading from TCP socket");
      close(client_fd);
      continue;
    }

    printf("Received from TCP client: %s\n", buf);

    bytes_sent = send(client_fd, buf, bytes_read, 0);
    if (bytes_sent == -1) {
      perror("Error writing to TCP socket");
    }

    close(client_fd);

    bytes_read = recvfrom(udp_fd, buf, sizeof(buf), 0,
                          (struct sockaddr *)&client_addr, &client_len);
    if (bytes_read == -1) {
      perror("Error receiving from UDP socket");
      continue;
    }

    printf("Received from UDP client: %s\n", buf);

    bytes_sent = sendto(udp_fd, buf, bytes_read, 0,
                        (struct sockaddr *)&client_addr, client_len);
    if (bytes_sent == -1) {
      perror("Error sending to UDP socket");
    }
  }

  close(tcp_fd);
  close(udp_fd);
  return EXIT_SUCCESS;
}
