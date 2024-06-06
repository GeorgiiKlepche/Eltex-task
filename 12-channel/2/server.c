#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  if (mkfifo("my_fifo", 0666) == -1) {
    printf("mkfifo");
    return 1;
  }

  int fifo_fd = open("my_fifo", O_WRONLY);
  if (fifo_fd == -1) {
    printf("open");
    return 1;
  }

  if (write(fifo_fd, "Hi!", 3) == -1) {
    printf("write");
    close(fifo_fd);
    return 1;
  }

  close(fifo_fd);

  printf("Server: Message written to FIFO\n");
  return 0;
}
