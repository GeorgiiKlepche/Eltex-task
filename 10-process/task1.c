#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid;

  pid = fork();

  if (pid < 0) {
    printf("Ошибка fork");
    return 1;
  } else if (pid == 0) {
    printf("Дочерний процесс: PID=%d, PPID=%d\n", getpid(), getppid());
    return 1;
  } else {
    printf("Родительский процесс: PID=%d, PPID=%d\n", getpid(), getppid());
    int status;
    waitpid(pid, &status, 0);
    printf("Статус завершения дочернего процесса: %d\n", WEXITSTATUS(status));
  }

  return 0;
}