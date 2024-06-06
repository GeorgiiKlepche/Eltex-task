#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  pid_t pid;
  char buffer[100];

  if (pipe(fd) == -1) {
    printf("Ошибка создания канала");
    return 1;
  }

  pid = fork();

  if (pid < 0) {
    printf("Ошибка fork");
    return 1;
  } else if (pid == 0) {
    close(fd[1]);
    read(fd[0], buffer, sizeof(buffer));
    printf("Дочерний процесс получил сообщение: %s\n", buffer);
    close(fd[0]);
    return 1;
  } else {
    close(fd[0]);
    write(fd[1], "Hi!", 3);
    close(fd[1]);
    wait(NULL);
  }
  return 0;
}
