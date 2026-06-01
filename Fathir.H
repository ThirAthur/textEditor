#ifndef FATHIR_H 
#define FATHIR_H

#define true 1
#define false 0
#define boolean unsigned char
#define First(L) ((L).First)
#define Last(L) ((L).Last)

#define clmn 80

typedef struct elmnt *address;
typedef struct elmnt{
    char info[clmn];
    address next;
    address prev;
} elmntL;

typedef struct list{
    address First;
    address Last;
} List;

boolean ListEmpty(List L);
void CreateList(List *L);
address Alokasi(char X[clmn]);
void DeAlokasi(address P);

void InsChar(List *L, char line[clmn], address *poscursor);
void InsChFirst(List *L, char line[clmn]);
void InsChAfter(List *L, char line[clmn], address *poscursor);
void InsChLast(List *L, char line[clmn]);

void DelChar(List *L, address *poscursor);
void DelChFirst(List *L);
void DelChLast(List *L);
void DelChMid(List *L, address *poscursor);

address searchLogic(List L, address *poscursor, char arr[100]);
void replaceLogic (List *L, address *poscursor, char arr[100], char repl[100]);

void insert_char(List *L, address *poscursor, int *c, char ch);
void delete_char(List *L, address *poscursor, int *c);
void new_line(List *L, address *poscursor, int *c);
void merge_line(List *L, address *poscursor, int *c);
void indention(List *L, address *poscursor, int *c);
void array_checker(List L, address poscursor);

#endif