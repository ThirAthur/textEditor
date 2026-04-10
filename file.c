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

        line[strcspn(line, "\n")] = '\0';

        strcpy(text_buffer[i], line);

        i++;
    }

    fclose(fp);
}

void save_as_file(const char *filename, char text_buffer[][COL])
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal menyimpan file\n");
        return;
    }

    for (int i = 0; i < ROW; i++) {

        if (text_buffer[i][0] == '\0')
            break;

        fprintf(fp, "%s", text_buffer[i]);

        if (i < ROW - 1 && text_buffer[i + 1][0] != '\0') {
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}