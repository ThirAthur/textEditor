#include <string.h>
#include "cursor.h"

static int get_document_max_row(char text[ROW][COL], int current_row) {
    int max_row = current_row;
    for (int i = 0; i < ROW; i++) {
        if (text[i][0] != '\0') {
            if (i > max_row) {
                max_row = i;
            }
        }
    }
    return max_row;
}

long line_length(char text[ROW][COL], int row)
{
    return strlen(text[row]);
}

void shift_line_right(char text[ROW][COL], int row, int col)
{
    int len = line_length(text, row);

    if (len >= COL - 1) {
        return;
    }

    for (int i = len; i >= col; i--) {
        text[row][i + 1] = text[row][i];
    }
}

void shift_line_left(char text[ROW][COL], int row, int col)
{
    int len = line_length(text, row);

    for (int i = col; i < len; i++) {
        text[row][i] = text[row][i + 1];
    }
}

void insert_empty_line(char text[ROW][COL], int row)
{
    for (int i = ROW - 1; i > row; i--) {
        strcpy(text[i], text[i - 1]);
    }
    text[row][0] = '\0';
}

void delete_line(char text[ROW][COL], int row)
{
    for (int i = row; i < ROW - 1; i++) {
        strcpy(text[i], text[i + 1]);
    }
    text[ROW - 1][0] = '\0';
}

void merge_line(char text[ROW][COL], int *row, int *col)
{
    int prev_len;
    int curr_len;

    if (*row <= 0) {
        return;
    }

    prev_len = line_length(text, *row - 1);
    curr_len = line_length(text, *row);

    if (prev_len + curr_len >= COL) {
        return;
    }

    strcat(text[*row - 1], text[*row]);
    delete_line(text, *row);

    (*row)--;
    *col = prev_len;
}

void cursor_move_left(char text[ROW][COL], int *row, int *col)
{
    if (*col > 0) {
        (*col)--;
    } else if (*row > 0) {
        (*row)--;
        *col = line_length(text, *row);
    }
}

void cursor_move_right(char text[ROW][COL], int *row, int *col)
{
    int len = line_length(text, *row);

    if (*col < len) {
        (*col)++;
    } else {
        int max_row = get_document_max_row(text, *row);
        if (*row < max_row) {
            (*row)++;
            *col = 0;
        }
    }
}

void cursor_move_up(char text[ROW][COL], int *row, int *col)
{
    if (*row > 0) {
        (*row)--;
        if (*col > line_length(text, *row)) {
            *col = line_length(text, *row);
        }
    }
}

void cursor_move_down(char text[ROW][COL], int *row, int *col)
{
    int max_row = get_document_max_row(text, *row);
    if (*row < max_row) {
        (*row)++;
        if (*col > line_length(text, *row)) {
            *col = line_length(text, *row);
        }
    }
}