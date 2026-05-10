#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "Fathir.H"

boolean ListEmpty(List L){
    return (First(L) == NULL);
}

void CreateList(List *L){
    First(*L) = NULL;
    Last(*L) = NULL;
}

address Alokasi(data X){
    address P;

    P = (address)malloc(sizeof(elmntL));
    if (P != NULL){
        P->info = X;
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

void InsChar(List *L, data X, address *poscursor){
    if(ListEmpty(*L) || *poscursor == First(*L)){
        InsChFirst(L, X);
        *poscursor = First(*L);
    }
    else if (*poscursor == Last(*L)){
        InsChLast(L, X);
        *poscursor = Last(*L);
    }
    else{
        InsChAfter(L, X, poscursor);
    }
}

void InsChFirst(List *L, data X){
    address P,Q;
    Q = First(*L);

    P = Alokasi(X);
    if (P != NULL){
        P->next = Q;
        P->prev = NULL;
        if (Q == NULL){
            Last(*L) = P;
        }else
        {
            Q->prev = P;
        }
        First(*L) = P;
    }
}

void InsChAfter(List *L, data X, address *poscursor){
    address P,Q;
    Q = (*poscursor)->next; // Menunjuk ke node setelah kursor

    P = Alokasi(X);
    if (P != NULL){
        P->next = Q;
        P->prev = *poscursor;

        (*poscursor)->next = P;
        Q->prev = P;

        *poscursor = P; // Pindahkan posisi kursor ke node yang baru disisipkan
    }
}

void InsChLast(List *L, data X){
    address P,Q;
    Q = Last(*L);

    P = Alokasi(X);
    if (P != NULL){
        P->next = NULL;
        P->prev = Q;
        Q->next = P;

        Last(*L) = P;
    }
}

void DelChar (List *L, address *poscursor){
    if(ListEmpty(*L) || *poscursor == First(*L)){
        if(ListEmpty(*L)){
            return;
        }else
        {
            DelChFirst(L);
            *poscursor = First(*L);
        }
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
    }else
    {
        Last(*L) = First(*L);
    }

    DeAlokasi(P);
}

void DelChLast(List *L){
    address P;

    P = Last(*L);

    Last(*L) = P->prev;
    Last(*L)->next = NULL;

    DeAlokasi(P);
}

void DelChMid(List *L, address *poscursor){
    address P,Q, Rem;

    Rem = *poscursor;
    P = (*poscursor)->next;
    Q = (*poscursor)->prev;

    P->prev = Q;
    Q->next = P;
    
    *poscursor = Q; // Mundurkan kursor

    DeAlokasi(Rem);
}

address searchLogic(List L, address *poscursor, char arr[100]){
    address P;
    int i;
    char temp[100]; // Deklarasi variabel temp untuk menampung data sementara

    P = First(L);
    while (P != NULL){
        i = 0;
        if(P->info == arr[i]){
            // Logika iteratif kamu dipertahankan di sini
            while(P->next == arr[i+1]){
                temp[i] = P->info;
                P = P->next;
                if (arr[i] == '\0'){
                    return P;
                }
                i++;
            }
        }
        P = P->next;
    }
    if(P == NULL && arr[i] != '\0'){
        printf("Tidak ditemukan\n");
        return NULL;
    }
    
    return NULL;
}

void callsearch_callfind(List *L, address *poscursor){
    address P;
    char arr[100];
    int len;

    printf("Masukkan kata yang ingin dicari: ");
    scanf("%s", arr);
    len = strlen(arr);

    P = searchLogic(*L, poscursor, arr);
    if (P != NULL){
        *poscursor = P;
        printf("Ditemukan!\n"); // Mengganti blok if() yang sebelumnya kosong
    }else{
        printf("Window alert: Not found\n");
    }
}