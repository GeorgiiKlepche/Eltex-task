#include "functions.h"

int main() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  char buf[BUF_SIZE];
  ssize_t bytes_read, bytes_sent;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Error creating socket");
    return EXIT_FAILURE;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(MAIN_PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Error binding");
    close(server_fd);
    return EXIT_FAILURE;
  }

  if (listen(server_fd, 5) == -1) {
    perror("Error listening");
    close(server_fd);
    return EXIT_FAILURE;
  }

  printf("Server listening on port %d...\n", MAIN_PORT);

  while (1) {
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
      perror("Error accepting connection");
      continue;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    bytes_read = recv(client_fd, buf, sizeof(buf), 0);
    if (bytes_read == -1) {
      perror("Error reading from socket");
      close(client_fd);
      continue;
    }

    printf("Received from client: %s\n", buf);

    bytes_sent = send(client_fd, buf, bytes_read, 0);
    if (bytes_sent == -1) {
      perror("Error writing to socket");
    }

    close(client_fd);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
