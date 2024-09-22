#include "files.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void LetoReadFile(char **buffer, const char *path)
{
    FILE *opened_file = fopen(path, "rb");
    if (opened_file == NULL)
    {
        fprintf(stderr, "Failed to open file at %s.\n", path);
        return;
    }

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

    *buffer = calloc((size_t)size + 1, 1);
    if (*buffer == NULL)
    {
        fprintf(stderr, "Failed file allocation.\n");
        return;
    }

    if (fread(*buffer, 1, (size_t)size, opened_file) != (size_t)size)
    {
        fprintf(stderr, "Failed to read file.\n");
        *buffer = NULL;
        return;
    }
    (*buffer)[size] = '\0';
    fclose(opened_file);
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

void LetoReadLines(char ***lines, size_t *line_count, const char *path)
{
    char *file_contents;
    LetoReadFile(&file_contents, path);
    if (file_contents == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        *lines = NULL;
        return;
    }

    char *current_segment = strtok(file_contents, "\n");
    while (current_segment != NULL)
    {
        *lines = realloc(*lines, sizeof(char *) * ++(*line_count));
        if (*lines == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            *lines = NULL;
            return;
        }

        (*lines)[(*line_count) - 1] = malloc(strlen(current_segment) + 1);
        strcpy((*lines)[(*line_count) - 1], current_segment);
        current_segment = strtok(NULL, "\n");
    }

    free(file_contents);
}

void LetoReadLinesV(char ***lines, size_t *line_count,
                    const char *path_format, ...)
{
    va_list args;
    va_start(args, path_format);

    char *full_path = malloc(LETO_FILE_PATH_MAX);
    vsnprintf(full_path, LETO_FILE_PATH_MAX, path_format, args);
    va_end(args);

    LetoReadLines(lines, line_count, full_path);
    free(full_path);
}
