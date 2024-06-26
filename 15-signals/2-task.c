#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool flag = true;

void signal_handler(int sig) {
  printf("Сигнал получен");
  flag = false;
}

int main() {
  struct sigaction sa;
  sa.sa_handler = signal_handler;

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, 2);
  sigprocmask(SIG_BLOCK, &set, NULL);

  sigaction(SIGUSR1, &sa, NULL);

  while (flag) {
  }
}