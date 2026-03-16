#include "Array.h"

void insert_char(char text[ROW][COL], int *r, int *c, char ch)
{
    if(ch == '\n'){
        (*r)++;
        *c = 0;
        return;
    }

    if(*r < ROW && *c < COL){
        text[*r][*c] = ch;
        (*c)++;
    }
}