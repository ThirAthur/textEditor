#ifndef CURSOR_H
#define CURSOR_H

#include "Array.h"

void cursor_insert_char(char text[ROW][COL], int *row, int *col, char ch);
void cursor_backspace(char text[ROW][COL], int *row, int *col);
void cursor_delete(char text[ROW][COL], int *row, int *col);
void cursor_enter(char text[ROW][COL], int *row, int *col);
void cursor_merge_line(char text[ROW][COL], int *row, int *col);
void cursor_move_left(char text[ROW][COL], int *row, int *col);
void cursor_move_right(char text[ROW][COL], int *row, int *col);
void cursor_move_up(char text[ROW][COL], int *row, int *col);
void cursor_move_down(char text[ROW][COL], int *row, int *col);

#endif
