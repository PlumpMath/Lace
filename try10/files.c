#include "files.h"
#include <stdio.h>
#include <stdlib.h>

char *file_read_all(const char *path) {
    printf("Trying to open file '%s'.\n", path);
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *contents = malloc(fsize + 1);
    fread(contents, fsize, 1, f);
    fclose(f);
    contents[fsize] = '\0';
    return contents;
}
