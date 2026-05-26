#include <stdio.h>
#include <string.h>
#include "Fathir.h"
#include "cursor.h"

void new_line(List *L, address *poscursor, int *c)
{
    char right_part[clmn] = {0};
    
    strcpy(right_part, &(*poscursor)->info[*c]); 
    (*poscursor)->info[*c] = '\0'; 
    InsChAfter(L, right_part, poscursor);
    
    *c = 0; 
}

void insert_char(List *L, address *poscursor, int *c, char ch)
{
    int len = strlen((*poscursor)->info);
    
    if (len >= clmn - 1) {
        new_line(L, poscursor, c);
    }

    len = strlen((*poscursor)->info);
    for (int i = len; i >= *c; i--) {
        (*poscursor)->info[i + 1] = (*poscursor)->info[i];
    }
    
    (*poscursor)->info[*c] = ch;
    (*c)++;
}

void merge_line(List *L, address *poscursor, int *c)
{
    if ((*poscursor)->prev != NULL) {
        address prevNode = (*poscursor)->prev;
        int prevLen = strlen(prevNode->info);
        int currLen = strlen((*poscursor)->info);

        if (prevLen + currLen < clmn) {
            strcat(prevNode->info, (*poscursor)->info);
            *c = prevLen;           
            DelChMid(L, poscursor); 
        }
    }
}

void delete_char(List *L, address *poscursor, int *c)
{
    if (*c > 0) {
        int len = strlen((*poscursor)->info);
        for (int i = *c - 1; i < len; i++) {
            (*poscursor)->info[i] = (*poscursor)->info[i + 1];
        }
        (*c)--;
    } 
    else if (*c == 0 && (*poscursor)->prev != NULL) {
        merge_line(L, poscursor, c);
    }
}

void indention(List *L, address *poscursor, int *c)
{
    for(int i = 0; i < 4; i++){
        insert_char(L, poscursor, c, ' ');
    }
}

void array_checker(List L, address poscursor)
{
    address P = First(L);
    int baris = 1;
    while (P != NULL) {
        printf("Baris %d: [%s]", baris, P->info);
        if (P == poscursor) {
            printf(" <-- KURSOR DI SINI");
        }
        printf("\n");
        P = P->next;
        baris++;
    }
    printf("------------------------------\n");
}