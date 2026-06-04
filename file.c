#include "file.h"
#include <stdio.h>
#include <string.h>


void create_file (List *L){
    CreateList(L);
}

void open_file (List *L, char *path){
    
    FILE *file = fopen (path, "r");

    if (file == NULL){
        printf("File tidak berhasil di buka!");
        return;
    }

    create_file(L);

    char temp[clmn];

    while (fgets(temp, sizeof(temp), file) != NULL){
        temp[strcspn(temp, "\n")] = 0;

        InsChLast(L, temp);
    }
    
    fclose(file);
}


void save_as_file (List *L, char *path){
    FILE *file = fopen (path, "w");

    if (file == NULL){
        printf("Gagal Menyimpan File!");
        return;
    }

    address P = First(*L);
    while ( P != NULL){
        fprintf(file, "%s", P->info);
        if (P->next != NULL){
            fprintf(file, "\n");

        }
        P = P->next;
    }
    fclose(file);
}

void close_file (List *L){
    create_file(L);
}
