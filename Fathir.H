#ifndef Fathir_H 
#define Fathir_H
#define true 1
#define false 0
#define boolean unsigned char
#define First(L) ((L).First)
#define Last(L) ((L).Last)

typedef char data;
typedef struct elmnt *address;
typedef struct elmnt{
    data info;
    address next;
    address prev;
}elmntL;

typedef struct list{
    address First;
    address Last;
}List;

boolean ListEmpty(List L);
void CreateList(List *L);
address Alokasi(data X);
void DeAlokasi(address P);

void InsChar(List *L, data X, address *poscursor);
void InsChFirst(List *L, data X);
void InsChAfter(List *L, data X, address *poscursor);
void InsChLast(List *L, data X);
/*Note: Fungsi InsChFirst, InsChAfter, dan InsChLast hanya boleh dipanggil melalui InsChar*/

void DelChar(List *L, address *poscursor);
void DelChFirst(List *L);
void DelChLast(List *L);
void DelChMid(List *L, address *poscursor);
/*Note: Fungsi DelChFirst, DelChMid, dan DelChLast hanya boleh dipanggil melalui DelChar*/

/*Search*/
address searchLogic(List L, address *poscursor, char arr[100]);
void callsearch_callfind(List *L, address *poscursor);
#endif