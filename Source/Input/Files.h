/**
 * @file Files.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides a basic API for opening, closing, and reading files.
 * @date 2024-10-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__FILES_H
#define LETO__FILES_H

// Standard variable arguments.
#include <stdarg.h>
// I/O functionality.
#include <stdio.h>

/**
 * ToggleFile
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Toggle a file's open/close state.
 *
 * @param file A pointer to a file object.
 * @param mode The mode in which to open the file. If you're trying to
 * close the file, this can be NULL.
 * @param path_format The format string for the file path. The starting
 * directory for this is the resource directory. If you're trying to close
 * this file, this can be NULL.
 * @return void -- Nothing.
 */
void LetoToggleFile(FILE **file, const char *mode, const char *path_format,
                    ...);

/**
 * ToggleFileV
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Toggle a file's open/close state, using a variable argument list
 * instead of straight-up variable arguments for the path value.
 *
 * @param file A pointer to a file object.
 * @param mode The mode in which to open the file. If you're trying to
 * close the file, this can be NULL.
 * @param path_format The format string for the file path. The starting
 * directory for this is the resource directory. If you're trying to close
 * this file, this can be NULL.
 * @param args The list of variable arguments we want passed into the @ref
 * path_format argument. This can be NULL if you're trying to close the
 * file.
 * @return void -- Nothing.
 */
void LetoToggleFileV(FILE **file, const char *mode,
                     const char *path_format, va_list args);

/**
 * ReadFile
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Read the contents of a file into the given buffer.
 *
 * @param buffer A pointer to a character array. This will be written over,
 * so anything currently allocated will be overwritten. Make sure to free
 * to prevent leaks.
 * @param read_size How many characters you want to read into the buffer.
 * However, if this is 0, we will just read the whole file into the buffer.
 * @param path_format The format string for the filepath.
 * @return void -- Nothing.
 */
void LetoReadFile(char **buffer, size_t read_size, const char *path_format,
                  ...);

#endif // LETO__FILES_H
