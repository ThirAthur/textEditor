#include <string.h>
#include "cursor.h"

void cursor_move_left(List *L, address *poscursor, int *c)
{
    if (*c > 0) {
        (*c)--;
    } else if ((*poscursor)->prev != NULL) {
        *poscursor = (*poscursor)->prev;
        *c = strlen((*poscursor)->info);
    }
}

void cursor_move_right(List *L, address *poscursor, int *c)
{
    if (*c < strlen((*poscursor)->info)) {
        (*c)++;
    } else if ((*poscursor)->next != NULL) {
        *poscursor = (*poscursor)->next;
        *c = 0;
    }
}

void cursor_move_up(List *L, address *poscursor, int *c)
{
    if ((*poscursor)->prev != NULL) {
        *poscursor = (*poscursor)->prev;
        if (*c > strlen((*poscursor)->info)) {
            *c = strlen((*poscursor)->info);
        }
    }
}

void cursor_move_down(List *L, address *poscursor, int *c)
{
    if ((*poscursor)->next != NULL) {
        *poscursor = (*poscursor)->next;
        if (*c > strlen((*poscursor)->info)) {
            *c = strlen((*poscursor)->info);
        }
    }
}