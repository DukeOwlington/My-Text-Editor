#ifndef TEXT_EDITOR_H_INCLUDED
#define TEXT_EDITOR_H_INCLUDED

#define CTRL(c) ((c) & 037)
#define WRITE 1
#define QUIT  2

int CountLength(int lineno);
int EditMode(void);
void InputMode(void);
int HandleInputMode(int character);
int HandleEditMode(int character);
void Info(void);
WINDOW *CreateNewwin(int height, int width, int starty, int startx);
void DestroyWin(WINDOW *local_win);
#endif // TEXT_EDITOR_H_INCLUDED
