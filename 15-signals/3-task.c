#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool flag = true;

void func(int sig) {
  if (sig == SIGUSR2) {
    printf("Получен сигнал SIGUSR2\n");
    flag = false;
  } else {
    printf("Получен сигнал SIGUSR1\n");
  }
}

int main() {
  sigset_t set;
  int sig;
  struct sigaction sa;

  sa.sa_handler = func;
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
  sigprocmask(SIG_BLOCK, &set, NULL);

  while (flag) {
    sigwait(&set, &sig);
  }

  printf("Программа завершена\n");
  return 0;
}
