#include <stdio.h>
#include <string.h>
#include "Array.h"
#include "cursor.h"

char text_buffer[ROW][COL] = {0};
int row_pos = 0;
int col_pos = 0;

void insert_char(char text[ROW][COL], int *r, int *c, char ch)
{
    if (*r >= ROW - 1) {
        return;
    }
    
    if (strlen(text[*r]) >= COL - 1) {
        new_line(text, r, c);
    }

    if (strlen(text[*r]) < COL - 1) {
        shift_line_right(text, *r, *c);
        text[*r][*c] = ch;
        (*c)++;
    }

}

void new_line(char text[ROW][COL], int *r, int *c)
{
    if(*r >= ROW - 1)
        return;

    insert_empty_line(text, *r + 1);
    strcpy(text[*r + 1], &text[*r][*c]);
    text[*r][*c] = '\0';

    (*r)++;
    *c = 0;
}

void delete_char(char text[ROW][COL], int *r, int *c)
{
    int tab = 0;
    if(*c >= 4){
        for (int i = 1; i <= 4; i++){
            if (text[*r][*c - i] == ' '){
                tab++;
            }
        }
    }
    if(tab == 4)
    {
        for(int i = 1; i <= 4; i++){
            (*c)--;
            shift_line_left(text, *r, *c);
        }
    }
    else if (*c > 0){
        (*c)--;
        shift_line_left(text, *r, *c);
    }
    else if(*r > 0){
    merge_line(text, r, c);
    }
}

void indention(char text[ROW][COL], int *r, int *c)
{
    for(int i = 0; i < 4; i++){
        insert_char(text, r, c, ' ');
    }
}

void set_cursor_position(char text[ROW][COL], int *r, int *c, int target_r, int target_c)
{
    if (target_r < 0 || target_r >= ROW || target_c < 0 || target_c >= COL) {
        return;
    }

    *r = target_r;
    *c = target_c;

    if (*c > strlen(text[*r])) {
        *c = strlen(text[*r]);
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