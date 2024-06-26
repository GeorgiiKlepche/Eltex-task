#include <pthread.h>
#include <semaphore.h>

#include "functions.h"

#define QUEUE_SIZE 10

sem_t empty, full;
pthread_mutex_t mutex;

char queue[QUEUE_SIZE][BUF_SIZE];
int front = 0, rear = 0;

void enqueue(char *data) {
  sem_wait(&empty);
  pthread_mutex_lock(&mutex);

  strcpy(queue[rear], data);
  rear = (rear + 1) % QUEUE_SIZE;

  pthread_mutex_unlock(&mutex);
  sem_post(&full);
}

void dequeue(char *data) {
  sem_wait(&full);
  pthread_mutex_lock(&mutex);

  strcpy(data, queue[front]);
  front = (front + 1) % QUEUE_SIZE;

  pthread_mutex_unlock(&mutex);
  sem_post(&empty);
}

void *handle_client(void *arg) {
  int client_fd = *((int *)arg);
  char buf[BUF_SIZE];
  ssize_t bytes_read;

  bytes_read = recv(client_fd, buf, sizeof(buf), 0);
  if (bytes_read == -1) {
    perror("Error reading from socket");
    close(client_fd);
    pthread_exit(NULL);
  }

  printf("Received from client: %s\n", buf);
  enqueue(buf);

  if (send(client_fd, buf, bytes_read, 0) == -1) {
    perror("Error writing to socket");
  }

  close(client_fd);
  pthread_exit(NULL);
}

int main() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  pthread_t thread;
  char buf[BUF_SIZE];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Error creating socket");
    return EXIT_FAILURE;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(MAIN_PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Error binding");
    close(server_fd);
    return EXIT_FAILURE;
  }

  if (listen(server_fd, 5) == -1) {
    perror("Error listening");
    close(server_fd);
    return EXIT_FAILURE;
  }

  sem_init(&empty, 0, QUEUE_SIZE);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&mutex, NULL);

  printf("Server listening on port %d...\n", MAIN_PORT);

  while (1) {
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
      perror("Error accepting connection");
      continue;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    if (pthread_create(&thread, NULL, handle_client, (void *)&client_fd) != 0) {
      perror("Error creating thread");
    }
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
