#include "file.h"
#include <stdio.h>
#include <string.h>

void create_file(char text_buffer[][COL])
{
    for(int i = 0; i < ROW; i++){
        text_buffer[i][0] = '\0';
    }
}

void open_file(const char *filename, char text_buffer[][COL])
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Gagal membuka file\n");
        return;
    }

    create_file(text_buffer);

    char line[COL];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL && i < ROW) {

        line[strcspn(line, "\0")] = '\0';

        strcpy(text_buffer[i], line);

        i++;
    }

    fclose(fp);
}