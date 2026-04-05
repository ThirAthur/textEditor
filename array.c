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

    if (*c >= COL - 1) {
        new_line(text, r, c);
    }

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

    if(*c < COL - 1){
        text[*r][*c] = '\n';
        text[*r][*c + 1] = '\0';
    }

    if(*r < ROW - 1){
        (*r)++;
        *c = 0;
    }
}

void delete_char(char text[ROW][COL], int *r, int *c)
{
    int tab = 0;
    if(*c > 0){
        for (int i = 1; i <= 4; i++){
            if (text[*r][*c - i] == ' '){
                tab++;
            }
        }
        if(tab == 4)
        {
            for(int i = 1; i <= 4; i++){
                text[*r][*c - i] = '\0';
            }
            *c -= 4;
        }
        else{
            (*c)--;
            text[*r][*c] = '\0';
        }
    } else if(*r > 0){
        (*r)--;
        *c = strlen(text[*r]);
        if (*c > 0 && text[*r][*c - 1] == '\n') {
            (*c)--;
            text[*r][*c] = '\0';
        }
    }
}

void indention(char text[ROW][COL], int *r, int *c)
{
    for(int i = 0; i < 4; i++){
        insert_char(text, r, c, ' ');
    }
}

void array_checker(char text[ROW][COL], int *r, int *c)
{
    // Loop only up to the current row to prevent "infinite" scrolling
    for (int i = 0; i <= *r && i < ROW; i++) 
    {
        for (int j = 0; j < COL; j++) 
        {
            char ch = text[i][j];

            if (ch == '\0') {
                printf("[\\0]");
                break;
            } 
            else if (ch == '\n') {
                printf("[\\n]");

            } 
            else {
                printf("[%c]", ch);
            }
        }
        printf("\n");
    }
    printf("------------------------------\n");
}