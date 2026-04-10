#ifndef CURSOR_H
#define CURSOR_H

#include "Array.h"

void shift_line_right(char text[ROW][COL], int row, int col);
void shift_line_left(char text[ROW][COL], int row, int col);
void insert_empty_line(char text[ROW][COL], int row);
void merge_line(char text[ROW][COL], int *row, int *col);
void cursor_move_left(char text[ROW][COL], int *row, int *col);
void cursor_move_right(char text[ROW][COL], int *row, int *col);
void cursor_move_up(char text[ROW][COL], int *row, int *col);
void cursor_move_down(char text[ROW][COL], int *row, int *col);

#endif