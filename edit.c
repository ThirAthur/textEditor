#include <string.h>
#include <time.h>
#include "edit.h"

#define MAX_HISTORY 100
#define MAX_SNAPSHOT 10000
#define SESSION_TIMEOUT_SEC 2

char internal_clipboard[CLIPBOARD_SIZE] = "";

typedef struct {
    char text[MAX_SNAPSHOT];
    int row;
    int col;
} HistoryItem;

typedef struct {
    HistoryItem data[MAX_HISTORY];
    int top;
} HistoryStack;

static HistoryStack undo_stack;
static HistoryStack redo_stack;

static int current_session = SESSION_NONE;
static time_t last_session_time = 0;

static void stack_init(HistoryStack *S)
{
    S->top = -1;
}

static int stack_empty(HistoryStack *S)
{
    return S->top == -1;
}

static void stack_push(HistoryStack *S, HistoryItem item)
{
    int i;

    if (S->top >= MAX_HISTORY - 1) {
        for (i = 1; i < MAX_HISTORY; i++) {
            S->data[i - 1] = S->data[i];
        }
        S->top = MAX_HISTORY - 2;
    }

    S->top++;
    S->data[S->top] = item;
}

static int stack_pop(HistoryStack *S, HistoryItem *item)
{
    if (stack_empty(S)) {
        return 0;
    }

    *item = S->data[S->top];
    S->top--;

    return 1;
}

static void stack_clear(HistoryStack *S)
{
    S->top = -1;
}

static void append_char(char *dest, int *idx, char ch)
{
    if (*idx < MAX_SNAPSHOT - 1) {
        dest[*idx] = ch;
        (*idx)++;
        dest[*idx] = '\0';
    }
}

static int get_cursor_row(List *L, address poscursor)
{
    int row = 0;
    address P = First(*L);

    while (P != NULL) {
        if (P == poscursor) {
            return row;
        }

        row++;
        P = P->next;
    }

    return 0;
}

static void make_snapshot(List *L, address poscursor, int col, HistoryItem *item)
{
    int idx = 0;
    address P = First(*L);

    item->text[0] = '\0';
    item->row = get_cursor_row(L, poscursor);
    item->col = col;

    while (P != NULL) {
        int i = 0;

        while (P->info[i] != '\0') {
            append_char(item->text, &idx, P->info[i]);
            i++;
        }

        if (P->next != NULL) {
            append_char(item->text, &idx, '\n');
        }

        P = P->next;
    }
}

static void clear_list(List *L)
{
    while (!ListEmpty(*L)) {
        DelChFirst(L);
    }

    CreateList(L);
}

static void restore_snapshot(List *L, address *poscursor, int *col, HistoryItem item)
{
    char line[clmn];
    int line_idx = 0;
    int text_idx = 0;
    int row_idx = 0;
    address target = NULL;

    clear_list(L);

    while (1) {
        char ch = item.text[text_idx];

        if (ch == '\n' || ch == '\0') {
            line[line_idx] = '\0';
            InsChLast(L, line);

            if (row_idx == item.row) {
                target = Last(*L);
            }

            row_idx++;
            line_idx = 0;

            if (ch == '\0') {
                break;
            }
        }
        else {
            if (line_idx < clmn - 1) {
                line[line_idx] = ch;
                line_idx++;
            }
            else {
                line[line_idx] = '\0';
                InsChLast(L, line);

                if (row_idx == item.row) {
                    target = Last(*L);
                }

                row_idx++;
                line_idx = 0;

                line[line_idx] = ch;
                line_idx++;
            }
        }

        text_idx++;
    }

    if (First(*L) == NULL) {
        char empty_line[clmn] = "";
        InsChFirst(L, empty_line);
    }

    if (target == NULL) {
        target = Last(*L);
    }

    *poscursor = target;
    *col = item.col;

    if (*col < 0) {
        *col = 0;
    }

    if (*col > (int)strlen((*poscursor)->info)) {
        *col = strlen((*poscursor)->info);
    }
}

static int snapshot_same(HistoryItem A, HistoryItem B)
{
    return strcmp(A.text, B.text) == 0 &&
           A.row == B.row &&
           A.col == B.col;
}

static void save_to_undo(List *L, address poscursor, int col)
{
    HistoryItem item;

    make_snapshot(L, poscursor, col, &item);

    if (!stack_empty(&undo_stack)) {
        if (snapshot_same(undo_stack.data[undo_stack.top], item)) {
            return;
        }
    }

    stack_push(&undo_stack, item);
    stack_clear(&redo_stack);
}

void edit_check_session_timeout(void)
{
    time_t now;

    if (current_session == SESSION_NONE) {
        return;
    }

    now = time(NULL);

    if (difftime(now, last_session_time) >= SESSION_TIMEOUT_SEC) {
        current_session = SESSION_NONE;
    }
}

void edit_end_session(void)
{
    current_session = SESSION_NONE;
}

void edit_begin_typing(List *L, address poscursor, int col)
{
    edit_check_session_timeout();

    if (current_session != SESSION_TYPING) {
        save_to_undo(L, poscursor, col);
        current_session = SESSION_TYPING;
    }

    last_session_time = time(NULL);
}

void edit_begin_delete(List *L, address poscursor, int col)
{
    edit_check_session_timeout();

    if (current_session != SESSION_DELETE) {
        save_to_undo(L, poscursor, col);
        current_session = SESSION_DELETE;
    }

    last_session_time = time(NULL);
}

void edit_begin_single_action(List *L, address poscursor, int col)
{
    edit_end_session();
    save_to_undo(L, poscursor, col);
}

void edit_init_history(void)
{
    stack_init(&undo_stack);
    stack_init(&redo_stack);
    current_session = SESSION_NONE;
    last_session_time = 0;
}

void edit_clear_history(void)
{
    edit_init_history();
}

void edit_undo(List *L, address *poscursor, int *col)
{
    HistoryItem current;
    HistoryItem target;

    edit_end_session();

    if (stack_empty(&undo_stack)) {
        return;
    }

    make_snapshot(L, *poscursor, *col, &current);
    stack_push(&redo_stack, current);

    stack_pop(&undo_stack, &target);
    restore_snapshot(L, poscursor, col, target);
}

void edit_redo(List *L, address *poscursor, int *col)
{
    HistoryItem current;
    HistoryItem target;

    edit_end_session();

    if (stack_empty(&redo_stack)) {
        return;
    }

    make_snapshot(L, *poscursor, *col, &current);
    stack_push(&undo_stack, current);

    stack_pop(&redo_stack, &target);
    restore_snapshot(L, poscursor, col, target);
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