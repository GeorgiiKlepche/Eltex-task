#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main() {
  int fifo_fd = open("my_fifo", O_RDONLY);
  if (fifo_fd == -1) {
    printf("open");
    return 1;
  }

  char buffer[BUFFER_SIZE];
  if (read(fifo_fd, buffer, BUFFER_SIZE) == -1) {
    printf("read");
    close(fifo_fd);
    return 1;
  }

  printf("Client: Received message: %s\n", buffer);

  close(fifo_fd);

  if (unlink("my_fifo") == -1) {
    printf("unlink");
    return 1;
  }

  return 0;
}
