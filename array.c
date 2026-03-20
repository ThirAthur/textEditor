#include <stdio.h>
#include <string.h>
#include "Array.h"

char text_buffer[ROW][COL] = {0};
int row_pos = 0;
int col_pos = 0;

void insert_char(char text[ROW][COL], int *r, int *c, char ch)
{
    if(*r >= ROW)
        return;

    if(*c < COL - 1){
        text[*r][*c] = ch;
        (*c)++;
        text[*r][*c] = '\0';
    }
}

void new_line(char text[ROW][COL], int *r, int *c)
{
    if(*r >= ROW)
        return;

    if(*c < COL){
        text[*r][*c] = '\0';
    }

    if(*r < ROW - 1){
        (*r)++;
        *c = 0;
    }
}

void delete_char(char text[ROW][COL], int *r, int *c)
{
    if(*c > 0){
         if(*c >= 4 &&
           text[*r][*c - 1] == ' ' &&
           text[*r][*c - 2] == ' ' &&
           text[*r][*c - 3] == ' ' &&
           text[*r][*c - 4] == ' ')
        {
            text[*r][*c - 1] = '\0';
            text[*r][*c - 2] = '\0';
            text[*r][*c - 3] = '\0';
            text[*r][*c - 4] = '\0';
            *c -= 4;
        }
        else{
            (*c)--;
            text[*r][*c] = '\0';
        }
    } else if(*r > 0){
        (*r)--;
        *c = strlen(text[*r]);
    }
}

void indention(char text[ROW][COL], int *r, int *c)
{
    for(int i = 0; i < 4; i++){
        insert_char(text, r, c, ' ');
    }
}

void array_checker(char ch)
{
    printf("%c", ch);
}