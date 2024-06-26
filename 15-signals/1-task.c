#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool flag = true;

void signal_1(int sig) {
  printf("Сигнал 1 получен\n");
  flag = false;
}

void signal_2(int sig) {
  printf("Сигнал 2 получен\n");
  flag = false;
}

int main() {
  signal(SIGUSR1, signal_1);
  struct sigaction sa;
  sa.sa_handler = signal_2;
  sigaction(SIGUSR2, &sa, NULL);

  while (flag) {
  }
}
