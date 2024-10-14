#include "Files.h"
#include <Output/Errors.h>
#include <Utilities/Macros.h>
#include <stdarg.h>
#include <string.h>

void LetoToggleFile(FILE **file, const char *mode, const char *path_format,
                    ...)
{
    if (*file != NULL)
    {
        fclose(*file);
        return;
    }

    char *full_format = NULL, *path = NULL;
    LETO_ALLOC_OR_FAIL(full_format, 14 + strlen(path_format));
    snprintf(full_format, LETO_MAX_PATH_LENGTH, ASSET_DIR "/%s",
             path_format);

    LETO_ALLOC_OR_FAIL(path, LETO_MAX_PATH_LENGTH);
    va_list args;
    va_start(args, path_format);
    vsnprintf(path, LETO_MAX_PATH_LENGTH, full_format, args);

    *file = fopen(path_format, mode);
    if (*file == NULL)
        LetoReportError(false, failed_file_open, LETO_FILE_CONTEXT);

    free(full_format);
    free(path);
}

void LetoToggleFileV(FILE **file, const char *mode,
                     const char *path_format, va_list args)
{
    if (*file != NULL)
    {
        fclose(*file);
        return;
    }

    char *path = NULL;
    LETO_ALLOC_OR_FAIL(path, LETO_MAX_PATH_LENGTH);
    sprintf(path, "%s/", ASSET_DIR);
    vsnprintf(path + sizeof(ASSET_DIR),
              LETO_MAX_PATH_LENGTH - sizeof(ASSET_DIR), path_format, args);

    *file = fopen(path, mode);
    if (*file == NULL)
        LetoReportError(false, failed_file_open, LETO_FILE_CONTEXT);

    free(path);
}

void LetoReadFile(char **buffer, size_t buffer_size,
                  const char *path_format, ...)
{
    FILE *file = NULL;

    va_list args;
    va_start(args, path_format);

    LetoToggleFileV(&file, "rb", path_format, args);
    if (file == NULL) return;

    size_t file_size = buffer_size;
    if (file_size == 0)
    {
        if (fseek(file, 0L, SEEK_END) == -1)
        {
            LetoReportError(false, failed_file_position,
                            LETO_FILE_CONTEXT);
            return;
        }

        file_size = (size_t)ftell(file);
        if (file_size == -1)
        {
            LetoReportError(false, failed_file_tell, LETO_FILE_CONTEXT);
            return;
        }

        // Reset the file positioner.
        if (fseek(file, 0L, SEEK_SET) == -1)
        {
            LetoReportError(false, failed_file_position,
                            LETO_FILE_CONTEXT);
            return;
        }

        LETO_ALLOC_OR_FAIL(*buffer, file_size + 1);
    }

    if (fread(*buffer, 1, file_size, file) != file_size)
    {
        LetoReportError(false, failed_file_read, LETO_FILE_CONTEXT);
        return;
    }
    (*buffer)[file_size] = '\0';
    LetoToggleFileV(&file, NULL, NULL, args);
    va_end(args);
}
