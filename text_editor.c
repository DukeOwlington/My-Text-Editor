#include <ncurses.h>
#include "text_editor.h"

/* Global value of current cursor position */
int row, col;

int CountLength(int lineno) {
  int linelen = COLS - 1;
  while (linelen >= 0 && mvinch(lineno, linelen) == ' ')
    linelen--;
  return linelen + 1;
}

int EditMode(void) {
  int character;
  int input;
  while (1) {
    character = getch();
    input = HandleEditMode(character);
    if (input == WRITE)
      return WRITE;
    else if (input == QUIT) {
      return QUIT;
    } else {
      move(row, col);
      refresh();
    }
  }
  return 0;
}

/*
 * Insert mode: accept characters and insert them.
 *  End with ^D or EIC
 */
void InputMode(void) {
  int character;
  move(LINES - 1, 1);
  clrtoeol();

  standout();
  mvaddstr(LINES - 1, 1, "ESC - exit input mode");
  mvaddstr(LINES - 1, COLS - 20, "INPUT MODE");
  standend();
  move(row, col);
  refresh();

  for (;;) {
    character = getch();
    if (HandleInputMode(character) == 1)  // ESC key
      break;
    move(row, col);
    refresh();
  }

  move(LINES - 1, 1);
  clrtoeol();
  move(row, col);

  standout();
  mvaddstr(LINES - 1, 1, "F1 - HELP");
  standend();
  refresh();
}

int HandleEditMode(int character) {
  /* Editor commands */
  switch (character) {

    /* hjkl and arrow keys: move cursor
     * in direction indicated */
    case KEY_F(1):
      Info();
      break;
    case 'h':
    case KEY_LEFT:
      if (col > 0)
        col--;
      break;

    case 'j':
    case KEY_DOWN:
      if (row < LINES - 1)
        row++;
      break;

    case 'k':
    case KEY_UP:
      if (row > 0)
        row--;
      break;

    case 'l':
    case KEY_RIGHT:
      if (col < COLS - 1)
        col++;
      break;
      /* i: enter input mode */
    case KEY_IC:
    case 'i':
      InputMode();
      break;
      /* x: delete current character */
    case KEY_DC:
    case 'x':
      delch();
      break;

      /* o: open up a new line and enter input mode */
    case KEY_IL:
    case 'o':
      move(++row, col = 0);
      insertln();
      InputMode();
      break;

      /* d: delete current line */
    case KEY_DL:
    case 'd':
      deleteln();
      break;

      /* ^L: redraw screen */
    case KEY_CLEAR:
    case CTRL('L'):
      wrefresh(curscr);
      break;

      /* w: write and quit */
    case 'w':
      return WRITE;
      /* q: quit without writing */
    case 'q':
      return QUIT;
      break;
    default:
      break;
  }
  return 0;
}

void Info() {
  WINDOW *my_win;
  int height = 12;
  int width = 60;
  int starty = (LINES - height) / 2; /* Calculating for a center placement */
  int startx = (COLS - width) / 2; /* of the window    */
  int wrow = 0, wcol = 0;
  wrow += 1;
  wcol += 7;
  my_win = create_newwin(height, width, starty, --startx);
  mvwaddstr(my_win, ++wrow, wcol, "q - quit without writing");
  mvwaddstr(my_win, ++wrow, wcol, "w - write and quit");
  mvwaddstr(my_win, ++wrow, wcol, "d - delete line");
  mvwaddstr(my_win, ++wrow, wcol, "x - delete current character");
  mvwaddstr(my_win, ++wrow, wcol, "i - input mode");
  mvwaddstr(my_win, ++wrow, wcol, "o - open up a new line and enter input mode");
  mvwaddstr(my_win, ++wrow, wcol, "CTRL + L - redraw screen");
  wrefresh(my_win);
  getch();
  destroy_win(my_win);
}

int HandleInputMode(int character) {
  if (character == 27) {  // ESC key
    return 1;
  }
  switch (character) {
    case KEY_LEFT:
      if (col > 0)
        col--;
      break;

    case KEY_DOWN:
      if (row < LINES - 1)
        row++;
      break;

    case KEY_UP:
      if (row > 0)
        row--;
      break;

    case KEY_RIGHT:
      if (col < COLS - 1)
        col++;
      break;

    case KEY_BACKSPACE:
      if (col != 0) {
        move(row, --col);
        delch();
      }
      break;

    default:
      insch(character);
      ++col;
      refresh();
      break;
  }
  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
   * for the vertical and horizontal
   * lines      */
  wrefresh(local_win); /* Show that box    */

  return local_win;
}

void destroy_win(WINDOW *local_win) {
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners
   * and so an ugly remnant of window.
   */
  wclear(local_win);
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  /* The parameters taken are
   * 1. win: the window on which to operate
   * 2. ls: character to be used for the left side of the window
   * 3. rs: character to be used for the right side of the window
   * 4. ts: character to be used for the top side of the window
   * 5. bs: character to be used for the bottom side of the window
   * 6. tl: character to be used for the top left corner of the window
   * 7. tr: character to be used for the top right corner of the window
   * 8. bl: character to be used for the bottom left corner of the window
   * 9. br: character to be used for the bottom right corner of the window
   */
  wrefresh(local_win);
  delwin(local_win);
}
