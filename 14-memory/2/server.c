#include "functions.h"

int users_count = 0;
int active_users = 0;
struct login_t login = {.stc_sem = NULL,
                        .cts_sem = NULL,
                        .srv_sem = NULL,
                        .usr_sem = NULL,
                        .shm_buf = NULL};
struct user_t *users = NULL;
struct timespec ts;

static void sigint_handler(int sig, siginfo_t *si, void *unused) { exit(0); }

void shutdown_server(void) {
  int index;
  int sec_index;

  if (login.shm_buf != NULL) {
    munmap(login.shm_buf, sizeof(struct log_msg_t));
  }
  if (login.stc_sem != NULL) {
    sem_close(login.stc_sem);
  }
  if (login.cts_sem != NULL) {
    sem_close(login.cts_sem);
  }
  if (login.srv_sem != NULL) {
    sem_close(login.srv_sem);
  }
  if (login.usr_sem != NULL) {
    sem_close(login.usr_sem);
  }

  sem_unlink(LOGIN_STOC);
  sem_unlink(LOGIN_CTOS);
  sem_unlink(LOGIN_LOG_USR);
  sem_unlink(LOGIN_LOG_SRV);

  if (users != NULL) {
    if (active_users != 0) {
      for (index = 0; index < users_count; ++index) {
        if (-1 == users[index].id) continue;

        users[index].shm_buf->stoc.comm = DISCONNECT_USR;
        sem_post(users[index].stc_sem);

        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
          printf("clock_gettime");
          exit(1);
        }
        ts.tv_sec += SERVER_TIMEOUT;
        if ((sem_timedwait(users[index].srv_sem, &ts)) == -1) {
          if (errno == ETIMEDOUT) {
            printf("User #%d has timed out\n", index);
          } else {
            printf("sem_timedout");
            exit(1);
          }
        }

        munmap(users[index].shm_buf, sizeof(struct usr_shm_t));
        sem_close(users[index].stc_sem);
        sem_close(users[index].cts_sem);
        sem_close(users[index].srv_sem);
        sem_close(users[index].usr_sem);

        shm_unlink(users[index].shm_name);
        sem_unlink(users[index].stc_sem_name);
        sem_unlink(users[index].cts_sem_name);
        sem_unlink(users[index].srv_sem_name);
        sem_unlink(users[index].usr_sem_name);
      }
    }

    free(users);
  }

  puts("Сервер завершил работу");
}

int server() {
  struct sigaction sa;
  char users_list[USER_MSG];
  char tmp_buf[USER_MSG];
  int users_num = DEF_ALLOC;
  int index;
  int sec_index;

  users = malloc(sizeof(struct user_t) * users_num);

  memset(users_list, 0, USER_MSG);

  sigfillset(&sa.sa_mask);
  sa.sa_sigaction = sigint_handler;
  if (sigaction(SIGINT, &sa, NULL) == -1) {
    printf("sigaction");
    exit(1);
  }

  atexit(shutdown_server);

  if ((login.shm_fd = shm_open(LOGIN_SHM, O_CREAT | O_RDWR, PERMS)) == -1) {
    printf("login_shm shm_open");
    exit(1);
  }
  if (ftruncate(login.shm_fd, sizeof(struct log_msg_t)) == -1) {
    printf("login_shm ftruncate");
    exit(1);
  }
  if ((login.shm_buf =
           mmap(NULL, sizeof(struct log_msg_t), PROT_READ | PROT_WRITE,
                MAP_SHARED, login.shm_fd, 0)) == MAP_FAILED) {
    printf("log_msg mmap");
    exit(1);
  }
  if ((login.stc_sem = sem_open(LOGIN_STOC, O_CREAT | O_RDWR, PERMS, 0)) ==
      SEM_FAILED) {
    printf("login stc_sem sem_open");
    exit(1);
  }
  if ((login.cts_sem = sem_open(LOGIN_CTOS, O_CREAT | O_RDWR, PERMS, 0)) ==
      SEM_FAILED) {
    printf("login cts_sem sem_open");
    exit(1);
  }
  if ((login.srv_sem = sem_open(LOGIN_LOG_SRV, O_CREAT | O_RDWR, PERMS, 0)) ==
      SEM_FAILED) {
    printf("login srv_sem sem_open");
    exit(1);
  }
  if ((login.usr_sem = sem_open(LOGIN_LOG_USR, O_CREAT | O_RDWR, PERMS, 1)) ==
      SEM_FAILED) {
    printf("login_log_usr_sem sem_open");
    exit(1);
  }

  while (1) {
    if (sem_trywait(login.cts_sem) == -1) {
      if (errno != EAGAIN) {
        printf("login cts_sem sem_trywait");
        exit(1);
      }
    } else {
      if (users_count >= users_num) {
        users_num += DEF_ALLOC;
        struct user_t *tmp = realloc(users, sizeof(struct user_t) * users_num);
        if (tmp == NULL) {
          printf("realloc");
          exit(1);
        }
      }
      users_count++;
      active_users++;

      users[users_count - 1].id = users_count;

      strncpy(users[users_count - 1].login, login.shm_buf->ctos, LOGIN_MSG);
      snprintf(users[users_count - 1].shm_name, LOGIN_MSG, USER_SHM,
               users[users_count - 1].id);
      snprintf(users[users_count - 1].stc_sem_name, LOGIN_MSG, USER_STOC,
               users[users_count - 1].id);
      snprintf(users[users_count - 1].cts_sem_name, LOGIN_MSG, USER_CTOS,
               users[users_count - 1].id);
      snprintf(users[users_count - 1].srv_sem_name, LOGIN_MSG, USER_SRV,
               users[users_count - 1].id);
      snprintf(users[users_count - 1].usr_sem_name, LOGIN_MSG, USER_USR,
               users[users_count - 1].id);

      if ((users[users_count - 1].usr_shm = shm_open(
               users[users_count - 1].shm_name, O_CREAT | O_RDWR, PERMS)) ==
          -1) {
        printf("usr_shm shm_open");
        exit(1);
      }
      if (ftruncate(users[users_count - 1].usr_shm, sizeof(struct usr_shm_t)) ==
          -1) {
        printf("usr_shm ftruncate");
        exit(1);
      }
      if ((users[users_count - 1].shm_buf = mmap(
               NULL, sizeof(struct usr_shm_t), PROT_READ | PROT_WRITE,
               MAP_SHARED, users[users_count - 1].usr_shm, 0)) == MAP_FAILED) {
        printf("shm_buf mmap");
        exit(1);
      }
      if ((users[users_count - 1].stc_sem =
               sem_open(users[users_count - 1].stc_sem_name, O_CREAT | O_RDWR,
                        PERMS, 0)) == SEM_FAILED) {
        printf("stc_sem sem_open");
        exit(1);
      }
      if ((users[users_count - 1].cts_sem =
               sem_open(users[users_count - 1].cts_sem_name, O_CREAT | O_RDWR,
                        PERMS, 0)) == SEM_FAILED) {
        printf("cts_sem sem_open");
        exit(1);
      }
      if ((users[users_count - 1].srv_sem =
               sem_open(users[users_count - 1].srv_sem_name, O_CREAT | O_RDWR,
                        PERMS, 0)) == SEM_FAILED) {
        printf("srv_sem sem_open");
        exit(1);
      }
      if ((users[users_count - 1].usr_sem =
               sem_open(users[users_count - 1].usr_sem_name, O_CREAT | O_RDWR,
                        PERMS, 0)) == SEM_FAILED) {
        printf("usr_sem sem_open");
        exit(1);
      }

      printf("User #%d \"%s\" has been registered\n", users[users_count - 1].id,
             users[users_count - 1].login);

      snprintf(login.shm_buf->stoc, LOGIN_MSG, "%d", users[users_count - 1].id);
      if (sem_post(login.stc_sem) == -1) {
        printf("sem_post");
        exit(1);
      }

      if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        printf("clock_gettime");
        exit(1);
      }
      ts.tv_sec += SERVER_TIMEOUT;
      if ((sem_timedwait(login.srv_sem, &ts)) == -1) {
        if (errno == ETIMEDOUT) {
          printf("User #%d \"%s\" has timed out\n", users[users_count - 1].id,
                 users[users_count - 1].login);

          munmap(users[users_count - 1].shm_buf, sizeof(struct usr_shm_t));
          shm_unlink(users[users_count - 1].shm_name);

          sem_close(users[users_count - 1].stc_sem);
          sem_close(users[users_count - 1].cts_sem);
          sem_close(users[users_count - 1].srv_sem);
          sem_close(users[users_count - 1].usr_sem);

          sem_unlink(users[users_count - 1].stc_sem_name);
          sem_unlink(users[users_count - 1].cts_sem_name);
          sem_unlink(users[users_count - 1].srv_sem_name);
          sem_unlink(users[users_count - 1].usr_sem_name);

          users[users_count - 1].id = -1;
        } else {
          printf("sem_timedwait");
          exit(1);
        }
      } else {
        printf("User #%d \"%s\" has succesfully connected\n",
               users[users_count - 1].id, users[users_count - 1].login);
        snprintf(tmp_buf, USER_MSG, "<%s>\n", users[users_count - 1].login);
        strncat(users_list, tmp_buf, USER_MSG);
        for (index = 0; index < users_count; ++index) {
          if (users[index].id == -1) continue;
          users[index].shm_buf->stoc.comm = UPD_USR;
          strncpy(users[index].shm_buf->stoc.msg, users_list, USER_MSG);
          sem_post(users[index].stc_sem);
        }
      }

      sem_post(login.usr_sem);
    }
    for (index = 0; index < users_count; ++index) {
      if (users[index].id == -1) continue;
      if (sem_trywait(users[index].cts_sem) == -1) {
        if (errno != EAGAIN) {
          printf("cts_sem sem_trywait");
          exit(1);
        }
      } else {
        switch (users[index].shm_buf->ctos.comm) {
          case MSGSND:
            printf("Received from \"%s\": <%s>\n", users[index].login,
                   users[index].shm_buf->ctos.msg);
            snprintf(tmp_buf, USER_MSG, "%s: %s", users[index].login,
                     users[index].shm_buf->ctos.msg);
            sem_post(users[index].usr_sem);

            for (sec_index = 0; sec_index < users_count; ++sec_index) {
              if (users[sec_index].id == -1) continue;

              users[sec_index].shm_buf->stoc.comm = MSGSND;
              strncpy(users[sec_index].shm_buf->stoc.msg, tmp_buf, USER_MSG);
              sem_post(users[sec_index].stc_sem);

              if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                printf("clock_gettime");
                exit(1);
              }
              ts.tv_sec += SERVER_TIMEOUT;
              if ((sem_timedwait(users[sec_index].srv_sem, &ts)) == -1) {
                if (errno == ETIMEDOUT) {
                  printf("User #%d has timed out\n", sec_index);
                } else {
                  printf("sem_timedout");
                  exit(1);
                }
              }
            }
            break;
          case SET_USR:
            strncpy(users[index].login, users[index].shm_buf->ctos.msg,
                    LOGIN_MSG);

            memset(users_list, 0, USER_MSG);
            for (sec_index = 0; sec_index < users_count; ++sec_index) {
              if (users[sec_index].id == -1) continue;
              snprintf(tmp_buf, USER_MSG, "<%s>\n", users[sec_index].login);
              strncat(users_list, tmp_buf, USER_MSG);
            }
            for (sec_index = 0; sec_index < users_count; ++sec_index) {
              if (users[sec_index].id == -1) continue;
              users[sec_index].shm_buf->stoc.comm = UPD_USR;
              strncpy(users[sec_index].shm_buf->stoc.msg, users_list, USER_MSG);

              sem_post(users[sec_index].stc_sem);

              if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                printf("clock_gettime");
                exit(1);
              }
              ts.tv_sec += SERVER_TIMEOUT;
              if ((sem_timedwait(users[sec_index].srv_sem, &ts)) == -1) {
                if (errno == ETIMEDOUT) {
                  printf("User #%d has timed out\n", sec_index);
                } else {
                  printf("sem_timedout");
                  exit(1);
                }
              }
            }
            printf("User #%d has been assigned new name: <%s>\n",
                   users[index].id, users[index].login);
            break;
          case REQ_USR:
            users[index].shm_buf->stoc.comm = UPD_USR;
            strncpy(users[index].shm_buf->stoc.msg, users_list, USER_MSG);

            sem_post(users[index].stc_sem);

            if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
              printf("clock_gettime");
              exit(1);
            }
            ts.tv_sec += SERVER_TIMEOUT;
            if ((sem_timedwait(users[index].srv_sem, &ts)) == -1) {
              if (errno == ETIMEDOUT) {
                printf("User #%d has timed out\n", sec_index);
              } else {
                printf("srv_sem sem_timedout");
                exit(1);
              }
            }
            printf("User %s has requested userlist\n", users[index].login);
            break;
          case DISCONNECT_USR:
            munmap(users[index].shm_buf, sizeof(struct usr_shm_t));
            shm_unlink(users[index].shm_name);

            sem_close(users[index].stc_sem);
            sem_close(users[index].cts_sem);
            sem_close(users[index].srv_sem);
            sem_close(users[index].usr_sem);

            sem_unlink(users[index].stc_sem_name);
            sem_unlink(users[index].cts_sem_name);
            sem_unlink(users[index].srv_sem_name);
            sem_unlink(users[index].usr_sem_name);

            printf("User #%d \"%s\" disconnected\n", users[index].id,
                   users[index].login);

            users[index].id = -1;
            active_users--;
            if (active_users <= 0) {
              exit(0);
            }
            memset(users_list, 0, USER_MSG);
            for (sec_index = 0; sec_index < users_count; ++sec_index) {
              if (users[sec_index].id == -1) continue;
              snprintf(tmp_buf, USER_MSG, "<%s>\n", users[sec_index].login);
              strncat(users_list, tmp_buf, USER_MSG);
            }
            for (sec_index = 0; sec_index < users_count; ++sec_index) {
              if (users[sec_index].id == -1) continue;
              users[sec_index].shm_buf->stoc.comm = UPD_USR;
              strncpy(users[sec_index].shm_buf->stoc.msg, users_list, USER_MSG);

              sem_post(users[sec_index].stc_sem);

              if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                printf("clock_gettime");
                exit(1);
              }
              ts.tv_sec += SERVER_TIMEOUT;
              if ((sem_timedwait(users[sec_index].srv_sem, &ts)) == -1) {
                if (errno == ETIMEDOUT) {
                  printf("User #%d has timed out\n", sec_index);
                } else {
                  printf("sem_timedout");
                  exit(1);
                }
              }
            }
            break;
          default:
            break;
        }
      }
    }
  }

  return 0;
}

int main() {
  server();

  return 0;
}