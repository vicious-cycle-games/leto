#include "files.h"
#include <malloc.h>
#include <stdarg.h>

void LetoReadFile(char **buffer, const char *path)
{
    FILE *opened_file = fopen(path, "rb");

    if (fseek(opened_file, 0L, SEEK_END) == -1)
    {
        fprintf(stderr, "Failed to set file positioner.\n");
        *buffer = NULL;
        return;
    }

    long size = ftell(opened_file);
    if (size == -1)
    {
        fprintf(stderr, "Failed to read file positioner.\n");
        *buffer = NULL;
        return;
    }
    // Reset the file positioner.
    if (fseek(opened_file, 0L, SEEK_SET) == -1)
    {
        fprintf(stderr, "Failed to reset file positioner.\n");
        *buffer = NULL;
        return;
    }

    *buffer = calloc(size + 1, 1);
    if (*buffer == NULL)
    {
        fprintf(stderr, "Failed file allocation.\n");
        return;
    }

    if (fread(*buffer, 1, size, opened_file) != size)
    {
        fprintf(stderr, "Failed to read file.\n");
        *buffer = NULL;
        return;
    }
    (*buffer)[size] = '\0';
}

void LetoReadFileV(char **buffer, const char *path_format, ...)
{
    va_list args;
    va_start(args, path_format);

    char *full_path = malloc(LETO_FILE_PATH_MAX);
    vsnprintf(full_path, LETO_FILE_PATH_MAX, path_format, args);
    va_end(args);

    LetoReadFile(buffer, full_path);
    free(full_path);
}
