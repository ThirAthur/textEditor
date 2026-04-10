#include <string.h>
#include "edit.h"

char internal_clipboard[CLIPBOARD_SIZE] = "";

static void insert_string_to_buffer(char text[ROW][COL], int *row, int *col, const char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '\n') {
            if (*row < ROW - 1) {
                (*row)++;
                *col = 0;
                text[*row][*col] = '\0';
            }
        } else {
            if (*row < ROW && *col < COL - 1) {
                text[*row][*col] = str[i];
                (*col)++;
                text[*row][*col] = '\0';
            }
        }
        i++;
    }
}

void copy_selected_text(GtkTextView *text_view)
{
    GtkTextBuffer *buffer;
    GtkTextIter start;
    GtkTextIter end;
    char *selected_text;

    buffer = gtk_text_view_get_buffer(text_view);

    if (gtk_text_buffer_get_selection_bounds(buffer, &start, &end)) {
        selected_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        strncpy(internal_clipboard, selected_text, CLIPBOARD_SIZE - 1);
        internal_clipboard[CLIPBOARD_SIZE - 1] = '\0';
        g_free(selected_text);
    }
}

void paste_clipboard_text(char text[ROW][COL], int *row, int *col)
{
    if (internal_clipboard[0] == '\0') {
        return;
    }

    insert_string_to_buffer(text, row, col, internal_clipboard);
}