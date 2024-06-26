#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define PERMS 0666
#define LOGIN_SHM "/login_shm"
#define LOGIN_STOC "/login_stoc_sem"
#define LOGIN_CTOS "/login_ctos_sem"
#define LOGIN_LOG_SRV "/login_log_srv_sem"
#define LOGIN_LOG_USR "/login_log_usr_sem"
#define USER_SHM "/client%d_shm"
#define USER_STOC "/client%d_stoc_sem"
#define USER_CTOS "/client%d_ctos_sem"
#define USER_SRV "/client%d_srv_sem"
#define USER_USR "/client%d_usr_sem"

#define DEF_ALLOC 10
#define SERVER_TIMEOUT 1
#define MSGSND 1
#define SET_USR 2
#define UPD_USR 3
#define REQ_USR 4
#define DISCONNECT_USR 5
#define LOGIN_MSG 80
#define USER_MSG 1024

struct log_msg_t {
  char stoc[LOGIN_MSG];
  char ctos[LOGIN_MSG];
};

struct login_t {
  int shm_fd;
  sem_t *stc_sem;
  sem_t *cts_sem;
  sem_t *srv_sem;
  sem_t *usr_sem;
  struct log_msg_t *shm_buf;
};

struct usr_msg_t {
  unsigned char comm;
  char msg[USER_MSG];
};

struct usr_shm_t {
  struct usr_msg_t stoc;
  struct usr_msg_t ctos;
};

struct user_t {
  int id;
  int usr_shm;
  sem_t *stc_sem;
  sem_t *cts_sem;
  sem_t *srv_sem;
  sem_t *usr_sem;
  char login[LOGIN_MSG];
  char shm_name[LOGIN_MSG];
  char stc_sem_name[LOGIN_MSG];
  char cts_sem_name[LOGIN_MSG];
  char srv_sem_name[LOGIN_MSG];
  char usr_sem_name[LOGIN_MSG];
  struct usr_shm_t *shm_buf;
};

struct windows_t {
  WINDOW *wnd;
  WINDOW *main_wnd;
  WINDOW *users_wnd;
  WINDOW *msg_wnd;
};

int server();
int client();
