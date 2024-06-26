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

void multicast_server() {
  int fd;
  struct sockaddr_in multicast_group, client;
  socklen_t size = sizeof(struct sockaddr_in);

  multicast_group.sin_family = AF_INET;
  multicast_group.sin_addr.s_addr = inet_addr("224.0.0.1");
  multicast_group.sin_port = htons(7777);

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    handle_error("ERR socket", -1);
  }

  if (sendto(fd, "Hello", 5, 0, (struct sockaddr *)&multicast_group, size) ==
      -1) {
    handle_error("ERR send", fd);
  }

  close(fd);
}

int main() {
  multicast_server();
  return 0;
}
