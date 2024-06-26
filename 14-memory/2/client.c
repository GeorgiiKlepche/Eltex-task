#include "functions.h"

struct winsize size;
struct login_t login = {.stc_sem = NULL,
                        .cts_sem = NULL,
                        .srv_sem = NULL,
                        .usr_sem = NULL,
                        .shm_buf = NULL};
struct user_t user = {.stc_sem = NULL,
                      .cts_sem = NULL,
                      .srv_sem = NULL,
                      .usr_sem = NULL,
                      .shm_buf = NULL};
struct windows_t windows;

void sig_winch(int signo) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

static void sigint_handler(int sig, siginfo_t *si, void *unused) { exit(0); }

void shutdown_client(void) {
  delwin(windows.main_wnd);
  delwin(windows.users_wnd);
  delwin(windows.msg_wnd);
  delwin(windows.wnd);
  endwin();

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

  if (user.shm_buf != NULL) {
    munmap(user.shm_buf, sizeof(struct usr_shm_t));
  }
  if (user.stc_sem != NULL) {
    sem_close(user.stc_sem);
  }
  if (user.cts_sem != NULL) {
    sem_close(user.cts_sem);
  }
  if (user.srv_sem != NULL) {
    sem_close(user.srv_sem);
  }
  if (user.usr_sem != NULL) {
    sem_close(user.usr_sem);
  }

  puts("Клиент завершил работу");
}

static void *msg_handler(void *args) {
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  while (1) {
    if (sem_trywait(user.stc_sem) == 0) {
      switch (user.shm_buf->stoc.comm) {
        case MSGSND:
          wprintw(windows.main_wnd, "%s\n", user.shm_buf->stoc.msg);
          wrefresh(windows.main_wnd);
          sem_post(user.srv_sem);
          break;
        case UPD_USR:
          wclear(windows.users_wnd);
          wprintw(windows.users_wnd, "%s", user.shm_buf->stoc.msg);
          wrefresh(windows.users_wnd);
          sem_post(user.srv_sem);
          break;
        case DISCONNECT_USR:
          exit(0);
          break;
        default:
          break;
      }
    }
  }
}

int _login_user(WINDOW **pwnd, char *name) {
  WINDOW *wnd;
  WINDOW *subwnd;
  int symbol;
  int index = 0;

  wnd = derwin(*pwnd, 7, 30, (size.ws_row / 2) - 3, (size.ws_col / 2) - 11);
  box(wnd, '|', '-');
  subwnd = derwin(wnd, 1, 28, 3, 1);
  wmove(wnd, 1, 8);
  wprintw(wnd, "Enter user name");
  wmove(wnd, 2, 1);
  whline(wnd, '*', 28);
  wmove(wnd, 4, 1);
  whline(wnd, '*', 28);
  wmove(wnd, 5, 2);
  wprintw(wnd, "Press ENTER to confirm...");

  wrefresh(wnd);

  keypad(subwnd, true);
  while (1) {
    symbol = wgetch(subwnd);
    if ('\n' == symbol) {
      break;
    } else if (127 > symbol && 32 <= symbol) {
      if (LOGIN_MSG < index) break;
      wprintw(subwnd, "%c", symbol);
      name[index] = symbol;
      index++;
    }
  }
  if (0 == index) {
    strncpy(name, "user", LOGIN_MSG);
  }

  wclear(wnd);
  wrefresh(wnd);
  delwin(subwnd);
  delwin(wnd);

  return 0;
}

int client() {
  struct sigaction sa;
  char tmp_buf[USER_MSG];
  int symbol;
  int index = 0;
  ssize_t ret;
  pthread_t tid;

  memset(tmp_buf, 0, USER_MSG);

  atexit(shutdown_client);

  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = sigint_handler;
  if (sigaction(SIGINT, &sa, NULL) == -1) {
    printf("sigaction");
    exit(1);
  }

  initscr();

  signal(SIGWINCH, sig_winch);

  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);

  cbreak();

  noecho();

  curs_set(1);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(1));
  refresh();

  windows.wnd = newwin(size.ws_row, size.ws_col, 0, 0);
  windows.main_wnd =
      derwin(windows.wnd, (size.ws_row - 7), (size.ws_col / 4) * 3 - 1, 1, 1);
  windows.users_wnd =
      derwin(windows.wnd, size.ws_row - 2, (size.ws_col / 4) - 2, 1,
             (size.ws_col / 4) * 3 + 1);
  windows.msg_wnd =
      derwin(windows.wnd, 4, (size.ws_col / 4) * 3 - 1, (size.ws_row - 5), 1);

  box(windows.wnd, '|', '-');
  wmove(windows.wnd, (size.ws_row - 6), 1);
  whline(windows.wnd, '-', (size.ws_col / 4) * 3 - 1);
  wmove(windows.wnd, 1, (size.ws_col / 4) * 3);
  wvline(windows.wnd, '|', size.ws_row - 2);

  wrefresh(windows.wnd);

  _login_user(&windows.wnd, user.login);

  wprintw(windows.main_wnd,
          "Please wait the client to establish "
          "connection with server for user '%s'\n",
          user.login);
  wrefresh(windows.main_wnd);

  while ((login.shm_fd = shm_open(LOGIN_SHM, O_RDWR, NULL)) == -1) {
    if (errno != ENOENT) {
      printf("shm_open");
      exit(1);
    }
  }
  if ((login.shm_buf =
           mmap(NULL, sizeof(struct log_msg_t), PROT_READ | PROT_WRITE,
                MAP_SHARED, login.shm_fd, 0)) == -1) {
    printf("log_msg mmap");
    exit(1);
  }
  while ((login.stc_sem = sem_open(LOGIN_STOC, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("login_stoc_sem sem_open");
      exit(1);
    }
  }
  while ((login.cts_sem = sem_open(LOGIN_CTOS, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("login_ctos_sem sem_open");
      exit(1);
    }
  }
  while ((login.srv_sem = sem_open(LOGIN_LOG_SRV, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("login_log_srv_sem sem_open");
      exit(1);
    }
  }
  while ((login.usr_sem = sem_open(LOGIN_LOG_USR, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("login_log_usr_sem sem_open");
      exit(1);
    }
  }

  sem_wait(login.usr_sem);

  strncpy(login.shm_buf->ctos, user.login, LOGIN_MSG);
  sem_post(login.cts_sem);

  sem_wait(login.stc_sem);

  user.id = atoi(login.shm_buf->stoc);
  snprintf(user.shm_name, LOGIN_MSG, USER_SHM, user.id);
  snprintf(user.stc_sem_name, LOGIN_MSG, USER_STOC, user.id);
  snprintf(user.cts_sem_name, LOGIN_MSG, USER_CTOS, user.id);
  snprintf(user.srv_sem_name, LOGIN_MSG, USER_SRV, user.id);
  snprintf(user.usr_sem_name, LOGIN_MSG, USER_USR, user.id);

  while ((user.usr_shm = shm_open(user.shm_name, O_RDWR, NULL)) == -1) {
    if (errno != ENOENT) {
      printf("usr_shm shm_open");
      exit(1);
    }
  }
  if ((user.shm_buf =
           mmap(NULL, sizeof(struct usr_shm_t), PROT_READ | PROT_WRITE,
                MAP_SHARED, user.usr_shm, 0)) == -1) {
    printf("shm_buf mmap");
    exit(1);
  }
  while ((user.stc_sem = sem_open(user.stc_sem_name, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("stc_sem sem_open");
      exit(1);
    }
  }
  while ((user.cts_sem = sem_open(user.cts_sem_name, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("cts_sem sem_open");
      exit(1);
    }
  }
  while ((user.srv_sem = sem_open(user.srv_sem_name, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("srv_sem sem_open");
      exit(1);
    }
  }
  while ((user.usr_sem = sem_open(user.usr_sem_name, O_RDWR)) == SEM_FAILED) {
    if (errno != ENOENT) {
      printf("usr_sem sem_open");
      exit(1);
    }
  }

  wprintw(windows.main_wnd, "Registered with ID: %d\n", user.id);
  wrefresh(windows.main_wnd);

  sem_post(login.srv_sem);

  munmap(login.shm_buf, sizeof(struct log_msg_t));
  sem_close(login.stc_sem);
  sem_close(login.cts_sem);
  sem_close(login.srv_sem);
  sem_close(login.usr_sem);

  pthread_create(&tid, NULL, msg_handler, NULL);

  keypad(windows.msg_wnd, true);
  while (1) {
    symbol = wgetch(windows.msg_wnd);
    if ('\n' == symbol) {
      if (0 == index) continue;

      wclear(windows.msg_wnd);
      wrefresh(windows.msg_wnd);

      user.shm_buf->ctos.comm = MSGSND;
      strncpy(user.shm_buf->ctos.msg, tmp_buf, USER_MSG);

      sem_post(user.cts_sem);
      sem_wait(user.usr_sem);

      index = 0;
      memset(tmp_buf, 0, USER_MSG);
    } else if (symbol >= 32 && symbol < 127) {
      if (80 < index) break;
      wprintw(windows.msg_wnd, "%c", (char)symbol);
      tmp_buf[index] = (char)symbol;
      index++;
    } else if (KEY_F(3) == symbol) {
      break;
    }
  }

  user.shm_buf->ctos.comm = DISCONNECT_USR;
  sem_post(user.cts_sem);

  pthread_cancel(tid);

  return 0;
}

int main() {
  client();

  return 0;
}