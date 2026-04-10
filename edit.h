#ifndef EDIT_H
#define EDIT_H

#include <gtk/gtk.h>
#include "Array.h"

#define CLIPBOARD_SIZE 4096

extern char internal_clipboard[CLIPBOARD_SIZE];

void copy_selected_text(GtkTextView *text_view);
void paste_clipboard_text(char text[ROW][COL], int *row, int *col);

#endif