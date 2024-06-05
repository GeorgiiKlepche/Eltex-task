#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 100

int main() {
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("Введите имя программы и опции (или 'exit' для выхода): ");
    fgets(input, MAX_INPUT_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    pid_t child_pid = fork();

    if (child_pid < 0) {
      printf("Ошибка fork");
      return 1;
    } else if (child_pid == 0) {
      char *args[MAX_INPUT_SIZE];
      char *token = strtok(input, " ");
      int arg_index = 0;
      while (token != NULL) {
        args[arg_index++] = token;
        token = strtok(NULL, " ");
      }
      args[arg_index] = NULL;

      execvp(args[0], args);
      printf("Ошибка execvp\n");
      return 1;
    } else {
      int status;
      waitpid(child_pid, &status, 0);
    }
  }

  return 0;
}
