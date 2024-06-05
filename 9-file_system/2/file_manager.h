#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define CWD_SIZE 100

struct window_t {
    int sel_dir;
    int scanned;
    struct dirent **namelist;
    char cwd[CWD_SIZE];
    WINDOW *path_wnd;
    WINDOW *main_wnd;
};

int file_manager();
