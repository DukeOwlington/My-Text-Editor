#include <stdio.h>
#include <ncurses.h>
#include "text_editor.h"

int main(int argc, char **argv) {
  extern void perror(), exit();
  int i, line_length, line = 0;
  int character;
  int result;
  FILE *file_descriptor;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    return 1;
  }

   file_descriptor = fopen(argv[1], "r");

   if (file_descriptor == NULL) {
     perror(argv[1]);
     return 2;
   }

  initscr();
  cbreak();
  nonl();
  noecho();
  idlok(stdscr, TRUE);
  keypad(stdscr, TRUE);
  ESCDELAY = 0;

  /* Read in the file */
  while ((character = getc(file_descriptor)) != EOF) {
    if (character == '\n')
      line++;
    if (line > LINES - 2)
      break;
    addch(character);
  }
  fclose(file_descriptor);

  standout();
  mvaddstr(LINES - 1, 1, "F1 - HELP");
  standend();
  move(0, 0);
  refresh();
  result = EditMode();

  if (result == 1) {
    /* Write out the file */
    file_descriptor = fopen(argv[1], "w");
    for (line = 0; line < LINES - 1; line++) {
      line_length = CountLength(line);
      for (i = 0; i < line_length; i++)
        putc(mvinch(line, i) & A_CHARTEXT, file_descriptor);
      putc('\n', file_descriptor);
    }
    fclose(file_descriptor);
  }
  endwin();
  return 0;
}

