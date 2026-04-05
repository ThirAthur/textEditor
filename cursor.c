#include <string.h>
#include "cursor.h"

static int line_length(char text[ROW][COL], int row)
{
    return strlen(text[row]);
}

static void shift_line_right(char text[ROW][COL], int row, int col)
{
    int len = line_length(text, row);

    if (len >= COL - 1) {
        return;
    }

    for (int i = len; i >= col; i--) {
        text[row][i + 1] = text[row][i];
    }
}

static void shift_line_left(char text[ROW][COL], int row, int col)
{
    int len = line_length(text, row);

    for (int i = col; i < len; i++) {
        text[row][i] = text[row][i + 1];
    }
}

static void insert_empty_line(char text[ROW][COL], int row)
{
    for (int i = ROW - 1; i > row; i--) {
        strcpy(text[i], text[i - 1]);
    }
    text[row][0] = '\0';
}

static void delete_line(char text[ROW][COL], int row)
{
    for (int i = row; i < ROW - 1; i++) {
        strcpy(text[i], text[i + 1]);
    }
    text[ROW - 1][0] = '\0';
}

void cursor_insert_char(char text[ROW][COL], int *row, int *col, char ch)
{
    if (*row < 0 || *row >= ROW) {
        return;
    }

    if (*col < 0) {
        *col = 0;
    }

    if (*col > line_length(text, *row)) {
        *col = line_length(text, *row);
    }

    if (line_length(text, *row) >= COL - 1) {
        return;
    }

    shift_line_right(text, *row, *col);
    text[*row][*col] = ch;
    (*col)++;
}

void cursor_backspace(char text[ROW][COL], int *row, int *col)
{
    if (*row < 0 || *row >= ROW) {
        return;
    }

    if (*col > 0) {
        shift_line_left(text, *row, *col - 1);
        (*col)--;
    } else if (*row > 0) {
        cursor_merge_line(text, row, col);
    }
}

void cursor_delete(char text[ROW][COL], int *row, int *col)
{
    int len;

    if (*row < 0 || *row >= ROW) {
        return;
    }

    len = line_length(text, *row);

    if (*col < len) {
        shift_line_left(text, *row, *col);
    } else if (*row < ROW - 1 && text[*row + 1][0] != '\0') {
        int current_len = line_length(text, *row);
        int next_len = line_length(text, *row + 1);

        if (current_len + next_len < COL) {
            strcat(text[*row], text[*row + 1]);
            delete_line(text, *row + 1);
        }
    }
}

void cursor_enter(char text[ROW][COL], int *row, int *col)
{
    int len;

    if (*row < 0 || *row >= ROW - 1) {
        return;
    }

    len = line_length(text, *row);

    if (*col < 0) {
        *col = 0;
    }

    if (*col > len) {
        *col = len;
    }

    insert_empty_line(text, *row + 1);
    strcpy(text[*row + 1], &text[*row][*col]);
    text[*row][*col] = '\0';

    (*row)++;
    *col = 0;
}

void cursor_merge_line(char text[ROW][COL], int *row, int *col)
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
    } else if (*row < ROW - 1 && text[*row + 1][0] != '\0') {
        (*row)++;
        *col = 0;
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
    if (*row < ROW - 1 && text[*row + 1][0] != '\0') {
        (*row)++;
        if (*col > line_length(text, *row)) {
            *col = line_length(text, *row);
        }
    }
}
