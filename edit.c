#include <string.h>
#include "edit.h"

char internal_clipboard[CLIPBOARD_SIZE] = "";

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

void paste_clipboard_text(List *L, address *poscursor, int *col)
{
    int i = 0;

    if (internal_clipboard[0] == '\0') {
        return;
    }

    while (internal_clipboard[i] != '\0') {
        if (internal_clipboard[i] == '\n') {
            new_line(L, poscursor, col);
        }
        else if (internal_clipboard[i] != '\r') {
            insert_char(L, poscursor, col, internal_clipboard[i]);
        }

        i++;
    }
}