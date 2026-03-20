#ifndef ARRAY_H
#define ARRAY_H

#define ROW 256
#define COL 256

extern char text_buffer[ROW][COL];
extern int row_pos;
extern int col_pos;

void insert_char(char text[ROW][COL], int *r, int *c, char ch);
void delete_char(char text[ROW][COL], int *r, int *c);
void indention(char text[ROW][COL], int *r, int *c);
void array_checker(char ch);
void new_line(char text[ROW][COL], int *r, int *c);

#endif