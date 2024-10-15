/**
 * @file Files.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Implements some basic file reading and opening functions.
 * @implements Files.h
 * @date 2024-10-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Files.h"            // Public interface parent
#include <Output/Errors.h>    // Error reporting
#include <Utilities/Macros.h> // Utility macros like LETO_MAX_PATH_LENGTH

#include <string.h> // Standard string utilities

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

void LetoReadFile(char **buffer, size_t read_size, const char *path_format,
                  ...)
{
    FILE *file = NULL;

    va_list args;
    va_start(args, path_format);
    // Open the file in "read binary" mode.
    LetoToggleFileV(&file, "rb", path_format, args);
    if (file == NULL) return;
    va_end(args);

    size_t file_size = read_size;
    if (read_size == 0)
    {
        // Go to the end of the file.
        if (fseek(file, 0L, SEEK_END) == -1)
        {
            LetoReportError(false, failed_file_position,
                            LETO_FILE_CONTEXT);
            return;
        }

        // Grab the current position, and since we're at the end, this is
        // the length of the file.
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
    }
    // Allocate the buffer to the needed size.
    LETO_ALLOC_OR_FAIL(*buffer, file_size + 1);

    if (fread(*buffer, 1, file_size, file) != file_size)
    {
        LetoReportError(false, failed_file_read, LETO_FILE_CONTEXT);
        return;
    }
    // Add in a terminating NUL character since fread does not.
    (*buffer)[file_size] = '\0';
    // Close the file.
    LetoToggleFile(&file, NULL, NULL);
}
