#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fathir.h"

boolean ListEmpty(List L){
    return (First(L) == NULL);
}

void CreateList(List *L){
    First(*L) = NULL;
    Last(*L) = NULL;
}

address Alokasi(char X[clmn]){
    address P;
    P = (address)malloc(sizeof(elmntL));
    if (P != NULL){
        strcpy(P->info, X);
        P->next = NULL;
        P->prev = NULL;
    }
    return(P);
}

void DeAlokasi(address P){
    if (P != NULL){
        free(P);
    }
}

void InsChar(List *L, char line[clmn], address *poscursor){
    if(ListEmpty(*L) || *poscursor == First(*L)){
        InsChFirst(L, line);
        *poscursor = First(*L);
    }
    else if (*poscursor == Last(*L)){
        InsChLast(L, line);
        *poscursor = Last(*L);
    }
    else{
        InsChAfter(L, line, poscursor);
    }
}

void InsChFirst(List *L,char line[clmn]){
    address P,Q;
    Q = First(*L);

    P = Alokasi(line);
    if (P != NULL){
        P->next = Q;
        P->prev = NULL;
        if (Q == NULL){
            Last(*L) = P;
        }else {
            Q->prev = P;
        }
        First(*L) = P;
    }
}

void InsChAfter(List *L, char line[clmn], address *poscursor){
    address P,Q;
    Q = (*poscursor)->next; 

    P = Alokasi(line);
    if (P != NULL){
        P->next = Q;
        P->prev = *poscursor;

        (*poscursor)->next = P;
        if(Q != NULL) {
            Q->prev = P;
        } else {
            Last(*L) = P;
        }
        *poscursor = P; 
    }
}

void InsChLast(List *L, char line[clmn]){
    address P,Q;
    Q = Last(*L);

    P = Alokasi(line);
    if (P != NULL){
        P->next = NULL;
        P->prev = Q;
        if (Q != NULL) Q->next = P;
        Last(*L) = P;
        if (First(*L) == NULL) First(*L) = P;
    }
}

void DelChar (List *L, address *poscursor){
    if(ListEmpty(*L)) return;
    
    if (*poscursor == First(*L)){
        DelChFirst(L);
        *poscursor = First(*L);
    }
    else if (*poscursor == Last(*L)){
        DelChLast(L);
        *poscursor = Last(*L);
    }
    else{
        DelChMid(L, poscursor);
    }
}

void DelChFirst(List *L){
    address P;
    P = First(*L);
    First(*L) = P->next;

    if (First(*L) != NULL){
        First(*L)->prev = NULL;
    }else {
        Last(*L) = NULL;
    }
    DeAlokasi(P);
}

void DelChLast(List *L){
    address P;
    P = Last(*L);
    Last(*L) = P->prev;
    if (Last(*L) != NULL) {
        Last(*L)->next = NULL;
    } else {
        First(*L) = NULL;
    }
    DeAlokasi(P);
}

void DelChMid(List *L, address *poscursor){
    address P,Q, Rem;
    Rem = *poscursor;
    P = (*poscursor)->next;
    Q = (*poscursor)->prev;

    P->prev = Q;
    Q->next = P;
    
    *poscursor = Q;
    DeAlokasi(Rem);
}