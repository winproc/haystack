#include <stdio.h>
#include <string.h>

#include "format.h"

void export_csv(char** content_buf, int limit) {

    FILE* file;
    fopen_s(&file, "export.csv", "w");

    int i = 0;
    while ((content_buf[i] != 0x0) && (i <= limit)) {
        size_t stlen = strlen(content_buf[i]); 

        content_buf[i][stlen - 1] = '\0'; // remove newline
        
        fwrite(content_buf[i], sizeof(char), strlen(content_buf[i]), file);

        if (content_buf[i + 1] != 0x0 && (i + 1) <= limit) {
            fputc(',', file);
        }

        i++;
    }

    fclose(file);

    return;
}

void export_json(char** content_buf, int limit) {

    FILE* file;
    fopen_s(&file, "export.json", "w");

    fputc('{', file); // starter

    fprintf(file, "\"%s\"", "test");

    fputc('}', file); // terminator

    fclose(file);

    return;
}