#include "file.h"
#include <stdio.h>
#include <string.h>

void create_file(char text_buffer[][COL])
{
    for (int i = 0; i < ROW; i++) {
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

void save_file(const char *filename, char text_buffer[][COL])
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal menyimpan file\n");
        return;
    }

    int last_row = -1;

    /* cari baris terakhir yang masih terpakai */
    for (int i = 0; i < ROW; i++) {
        if (text_buffer[i][0] != '\0') {
            last_row = i;
        }
    }

    /* kalau semua kosong, buat file kosong */
    if (last_row == -1) {
        fclose(fp);
        return;
    }

    /* simpan semua baris sampai baris terakhir, termasuk baris kosong di tengah */
    for (int i = 0; i <= last_row; i++) {
        fprintf(fp, "%s", text_buffer[i]);

        if (i < last_row) {
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}

void save_as_file(const char *filename, char text_buffer[][COL])
{
    save_file(filename, text_buffer);
}