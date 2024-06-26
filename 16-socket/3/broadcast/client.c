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

void setup_client_address(struct sockaddr_in *address) {
  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_BROADCAST;
  address->sin_port = htons(7777);
}

void broadcast_client() {
  int fd;
  struct sockaddr_in client;
  char buf[15];
  socklen_t size = sizeof(struct sockaddr_in);

  setup_client_address(&client);

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    handle_error("ERR socket", -1);
  }

  if (bind(fd, (struct sockaddr *)&client, size) == -1) {
    handle_error("ERR bind", fd);
  }

  if (recvfrom(fd, buf, 5, 0, (struct sockaddr *)NULL, NULL) == -1) {
    handle_error("ERR recv", fd);
  } else {
    printf("%s\n", buf);
  }

  close(fd);
}

int main() {
  broadcast_client();
  return 0;
}
