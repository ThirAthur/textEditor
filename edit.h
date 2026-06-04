#ifndef EDIT_H
#define EDIT_H

#include <gtk/gtk.h>
#include "Fathir.h"

#define CLIPBOARD_SIZE 4096

extern char internal_clipboard[CLIPBOARD_SIZE];

typedef enum {
    SESSION_NONE = 0,
    SESSION_TYPING = 1,
    SESSION_DELETE = 2
} EditSession;

void copy_selected_text(GtkTextView *text_view);
void paste_clipboard_text(List *L, address *poscursor, int *col);

void edit_init_history(void);
void edit_clear_history(void);

void edit_begin_typing(List *L, address poscursor, int col);
void edit_begin_delete(List *L, address poscursor, int col);
void edit_begin_single_action(List *L, address poscursor, int col);
void edit_end_session(void);
void edit_check_session_timeout(void);

void edit_undo(List *L, address *poscursor, int *col);
void edit_redo(List *L, address *poscursor, int *col);

#endif