#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void create_process(int processNumber) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Ошибка fork");
    exit(1);
  } else if (pid == 0) {
    printf("Процесс%d: PID=%d, PPID=%d\n", processNumber, getpid(), getppid());
    if (processNumber == 1) {
      create_process(3);
      create_process(4);
    } else if (processNumber == 2) {
      create_process(5);
    }
    exit(0);
  } else {
    wait(NULL);
  }
}

int main() {
  create_process(1);
  create_process(2);

  return 0;
}
