#ifndef CURSOR_H
#define CURSOR_H

#include "Fathir.h"

void cursor_move_left(List *L, address *poscursor, int *c);
void cursor_move_right(List *L, address *poscursor, int *c);
void cursor_move_up(List *L, address *poscursor, int *c);
void cursor_move_down(List *L, address *poscursor, int *c);

#endif