#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

void handle_error(const char *msg, int sock) {
  printf("%s %d\n", msg, errno);
  if (sock != -1) {
    close(sock);
  }
  exit(EXIT_FAILURE);
}

void setup_server_address(struct sockaddr_in *address) {
  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_BROADCAST;
  address->sin_port = htons(7777);
}

void broadcast_server() {
  int fd;
  struct sockaddr_in server, client;
  socklen_t size = sizeof(struct sockaddr_in);

  setup_server_address(&client);

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    handle_error("ERR socket", -1);
  }

  int flag = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (void *)&flag,
                 (socklen_t)sizeof(flag)) == -1) {
    handle_error("ERR setsockopt", fd);
  }

  if (sendto(fd, "Hello", 5, 0, (struct sockaddr *)&client, size) == -1) {
    handle_error("ERR send", fd);
  }

  close(fd);
}

int main() {
  broadcast_server();
  return 0;
}
