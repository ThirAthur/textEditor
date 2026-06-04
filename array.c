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
    array_checker(*L);
}

void wrapWord (List *L, address *poscursor, int *c)
{
    int oriC = *c;
    while ((*poscursor)->info[*c] != ' ' && *c > 0) {
        (*c)--;
    }
    if (*c == 0) {
        *c = oriC;  
        new_line(L, poscursor, c);
    } else {
        (*c)++;
        new_line(L, poscursor, c);
        while ((*poscursor)->info[*c] != '\0') {
            (*c)++;
        }
    }
}

void insert_char(List *L, address *poscursor, int *c, char ch)
{
    int len = strlen((*poscursor)->info);
    
    if (len >= clmn - 1) {
        wrapWord(L, poscursor, c);
    }

    len = strlen((*poscursor)->info);
    for (int i = len; i >= *c; i--) {
        (*poscursor)->info[i + 1] = (*poscursor)->info[i];
    }
    
    (*poscursor)->info[*c] = ch;
    (*c)++;
    array_checker(*L);
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
            DelChar(L, poscursor); 
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
    array_checker(*L);
}

void indention(List *L, address *poscursor, int *c)
{
    for(int i = 0; i < 4; i++){
        insert_char(L, poscursor, c, ' ');
    }
}

boolean searchLogic(List L, address *poscursor, int *col_pos, char arr[100])
{
    address P;
    int sCol;
    
    if (*poscursor == NULL) {
        P = First(L);
        sCol = 0;
    } else {
        P = *poscursor;
        sCol = *col_pos; 
    }

    while (P != NULL) {
        if (strstr(P->info + sCol, arr) != NULL) {
            *poscursor = P;
            *col_pos = sCol;
            return 1;
        }
        P = P->next;
        sCol = 0;
    }
    return 0;
}

void replaceLogic(List *L, address *poscursor, int col_pos, char search_term[100], char replace_term[100])
{
    address P = *poscursor;
    if (P != NULL) {
        char newText[clmn] = {0}; 

        // 1. Salin bagian teks SEBELUM kata yang diganti (berdasarkan posisi kursor)
        strncpy(newText, P->info, col_pos);
        newText[col_pos] = '\0';

        // 2. Tempelkan kata penggantinya
        strcat(newText, replace_term);

        // 3. Tempelkan sisa teks SETELAH kata yang diganti
        strcat(newText, P->info + col_pos + strlen(search_term));

        // 4. Salin kembali ke dalam linked list
        strncpy(P->info, newText, clmn - 1);
        P->info[clmn - 1] = '\0'; 
    }
}

void array_checker(List L)
{
    printf("\033[H");
    
    address P = First(L);
    while (P != NULL) {
        printf("[ %s ]", P->info);
        printf("\n");
        P = P->next;
    }
    printf("------------------------------\n");

    printf("\033[0J"); 
}