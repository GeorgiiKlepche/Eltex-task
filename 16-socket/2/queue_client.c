#include "functions.h"

int main() {
  int client_fd;
  struct sockaddr_in server_addr;
  char buf[BUF_SIZE];
  ssize_t bytes_read, bytes_sent;

  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("Error creating socket");
    return EXIT_FAILURE;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(MAIN_PORT);

  if (connect(client_fd, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Error connecting to server");
    close(client_fd);
    return EXIT_FAILURE;
  }

  printf("Connected to server\n");

  printf("Enter message to send: ");
  fgets(buf, sizeof(buf), stdin);

  bytes_sent = send(client_fd, buf, strlen(buf), 0);
  if (bytes_sent == -1) {
    perror("Error sending message");
    close(client_fd);
    return EXIT_FAILURE;
  }

  bytes_read = recv(client_fd, buf, sizeof(buf), 0);
  if (bytes_read == -1) {
    perror("Error receiving message");
    close(client_fd);
    return EXIT_FAILURE;
  }

  printf("Received from server: %.*s\n", (int)bytes_read, buf);

  close(client_fd);
  return EXIT_SUCCESS;
}
