#include "functions.h"

int main() {
  int tcp_fd, udp_fd;
  struct sockaddr_in server_addr;
  char buf[BUF_SIZE];
  ssize_t bytes_read, bytes_sent;

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
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(MAIN_PORT);

  if (connect(tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Error connecting to TCP server");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  printf("Connected to TCP server\n");

  printf("Enter message to send over TCP: ");
  fgets(buf, sizeof(buf), stdin);

  bytes_sent = send(tcp_fd, buf, strlen(buf), 0);
  if (bytes_sent == -1) {
    perror("Error sending message over TCP");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  bytes_read = recv(tcp_fd, buf, sizeof(buf), 0);
  if (bytes_read == -1) {
    perror("Error receiving message over TCP");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  printf("Received from TCP server: %.*s\n", (int)bytes_read, buf);

  printf("Enter message to send over UDP: ");
  fgets(buf, sizeof(buf), stdin);

  bytes_sent = sendto(udp_fd, buf, strlen(buf), 0,
                      (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bytes_sent == -1) {
    perror("Error sending message over UDP");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  bytes_read = recvfrom(udp_fd, buf, sizeof(buf), 0, NULL, NULL);
  if (bytes_read == -1) {
    perror("Error receiving message over UDP");
    close(tcp_fd);
    close(udp_fd);
    return EXIT_FAILURE;
  }

  printf("Received from UDP server: %.*s\n", (int)bytes_read, buf);

  close(tcp_fd);
  close(udp_fd);
  return EXIT_SUCCESS;
}
