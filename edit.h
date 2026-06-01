#ifndef EDIT_H
#define EDIT_H

#include <gtk/gtk.h>
#include "Fathir.h"

#define CLIPBOARD_SIZE 4096

extern char internal_clipboard[CLIPBOARD_SIZE];

void copy_selected_text(GtkTextView *text_view);
void paste_clipboard_text(List *L, address *poscursor, int *col);

#endif