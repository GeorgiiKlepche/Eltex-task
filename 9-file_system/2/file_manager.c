#include "file_manager.h"

struct winsize term_size;

void handle_sigwinch(int signal) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&term_size);
  resizeterm(term_size.ws_row, term_size.ws_col);
}

void update_window(struct window_t *pane, int highlight) {
  wclear(pane->main_wnd);
  for (int i = 1; i < pane->scanned; ++i) {
    if (i == pane->sel_dir && highlight) {
      wattron(pane->main_wnd, COLOR_PAIR(2));
    }
    wprintw(pane->main_wnd, "%s\n", pane->namelist[i]->d_name);
    if (i == pane->sel_dir && highlight) {
      wattroff(pane->main_wnd, COLOR_PAIR(2));
    }
  }
  wrefresh(pane->main_wnd);
}

void update_path_window(struct window_t *pane, int highlight) {
  wclear(pane->path_wnd);
  if (highlight) {
    wattron(pane->path_wnd, COLOR_PAIR(2));
  }
  wprintw(pane->path_wnd, "%s", pane->cwd);
  if (highlight) {
    wattroff(pane->path_wnd, COLOR_PAIR(2));
  }
  wrefresh(pane->path_wnd);
}

void change_directory(struct window_t *pane) {
  if (pane->namelist[pane->sel_dir]->d_type != 4) {
    return;
  }

  chdir(pane->namelist[pane->sel_dir]->d_name);
  getcwd(pane->cwd, CWD_SIZE);
  if (NULL == pane->cwd) {
    perror("getcwd");
  }

  pane->sel_dir = 1;

  for (int i = 0; i < pane->scanned; ++i) {
    free(pane->namelist[i]);
  }
  free(pane->namelist);

  pane->scanned = scandir(pane->cwd, &pane->namelist, NULL, alphasort);
}

int file_manager() {
  struct window_t panes[2] = {{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}};
  WINDOW *main_wnd;
  WINDOW *left_wnd;
  WINDOW *right_wnd;
  int key_input;
  int active_pane = 0;

  getcwd(panes[0].cwd, CWD_SIZE);
  if (NULL == panes[0].cwd) {
    perror("getcwd");
  }
  strncpy(panes[1].cwd, panes[0].cwd, CWD_SIZE);

  panes[0].scanned = scandir(panes[0].cwd, &panes[0].namelist, NULL, alphasort);
  panes[1].scanned = scandir(panes[1].cwd, &panes[1].namelist, NULL, alphasort);

  initscr();
  signal(SIGWINCH, handle_sigwinch);

  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&term_size);

  cbreak();
  noecho();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  bkgd(COLOR_PAIR(1));
  refresh();

  main_wnd = newwin(term_size.ws_row, term_size.ws_col, 0, 0);
  left_wnd = derwin(main_wnd, term_size.ws_row, (term_size.ws_col / 2), 0, 0);
  box(left_wnd, '|', '-');
  wmove(left_wnd, 2, 1);
  whline(left_wnd, '-', (term_size.ws_col / 2) - 2);
  panes[0].main_wnd = derwin(left_wnd, (term_size.ws_row - 4),
                             (term_size.ws_col / 2) - 2, 3, 1);
  panes[0].path_wnd = derwin(left_wnd, 1, (term_size.ws_col / 2) - 2, 1, 1);

  right_wnd = derwin(main_wnd, term_size.ws_row, (term_size.ws_col / 2), 0,
                     (term_size.ws_col / 2));
  box(right_wnd, '|', '-');
  wmove(right_wnd, 2, 1);
  whline(right_wnd, '-', (term_size.ws_col / 2) - 2);
  panes[1].main_wnd = derwin(right_wnd, (term_size.ws_row - 4),
                             (term_size.ws_col / 2) - 2, 3, 1);
  panes[1].path_wnd = derwin(right_wnd, 1, (term_size.ws_col / 2) - 2, 1, 1);

  update_path_window(&panes[0], 1);
  update_path_window(&panes[1], 0);

  for (int i = 0; i < 2; ++i) {
    update_window(&panes[i], i == active_pane);
  }

  wrefresh(main_wnd);
  keypad(panes[0].main_wnd, true);
  keypad(panes[1].main_wnd, true);

  while (1) {
    key_input = wgetch(panes[active_pane].main_wnd);

    if ('\t' == key_input) {
      update_path_window(&panes[active_pane], 0);
      update_window(&panes[active_pane], 0);

      active_pane = 0 == active_pane ? 1 : 0;

      update_path_window(&panes[active_pane], 1);
      update_window(&panes[active_pane], 1);
    } else if (259 == key_input) {
      if (panes[active_pane].sel_dir > 1) {
        panes[active_pane].sel_dir--;
        update_window(&panes[active_pane], 1);
      }
    } else if (258 == key_input) {
      if (panes[active_pane].sel_dir < panes[active_pane].scanned - 1) {
        panes[active_pane].sel_dir++;
        update_window(&panes[active_pane], 1);
      }
    } else if ('\n' == key_input) {
      change_directory(&panes[active_pane]);
      update_path_window(&panes[active_pane], 1);
      update_window(&panes[active_pane], 1);
    } else if (KEY_F(3) == key_input) {
      break;
    }
  }

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < panes[i].scanned; ++j) {
      free(panes[i].namelist[j]);
    }
    free(panes[i].namelist);
  }

  endwin();
  exit(EXIT_SUCCESS);
}
