#ifndef LETO__FILES_H
#define LETO__FILES_H

#include <stdarg.h>
#include <stdio.h>

#define LETO_MAX_PATH_LENGTH 256

void LetoToggleFile(FILE **file, const char *mode, const char *path_format,
                    ...);
void LetoToggleFileV(FILE **file, const char *mode,
                     const char *path_format, va_list args);

/**
 * @note if buffer_size is 0, read out full file into buffer, overwriting
 * whatever is already in there
 */
void LetoReadFile(char **buffer, size_t buffer_size,
                  const char *path_format, ...);

#endif // LETO__FILES_H
