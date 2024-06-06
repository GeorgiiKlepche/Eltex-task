#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 100
#define MAX_ARGS 10

void execute_command(char *cmd) {
  char *args[MAX_ARGS];
  char *token = strtok(cmd, " ");
  int arg_index = 0;

  while (token != NULL) {
    args[arg_index++] = token;
    token = strtok(NULL, " ");
  }
  args[arg_index] = NULL;

  execvp(args[0], args);
  printf("Ошибка execvp\n");
  exit(1);
}

int main() {
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("Введите команду (или 'exit' для выхода): ");
    fgets(input, MAX_INPUT_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    char *commands[MAX_ARGS];
    char *command = strtok(input, "|");
    int command_count = 0;

    while (command != NULL) {
      commands[command_count++] = command;
      command = strtok(NULL, "|");
    }

    int pipefd[2], in_fd = 0;

    for (int i = 0; i < command_count; i++) {
      pipe(pipefd);
      pid_t child_pid = fork();

      if (child_pid < 0) {
        printf("Ошибка fork\n");
        return 1;
      } else if (child_pid == 0) {
        dup2(in_fd, 0);

        if (i < command_count - 1) {
          dup2(pipefd[1], 1);
        }

        close(pipefd[0]);
        execute_command(commands[i]);
      } else {
        wait(NULL);
        close(pipefd[1]);
        in_fd = pipefd[0];
      }
    }
  }

  return 0;
}
