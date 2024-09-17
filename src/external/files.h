/**
 * @file shader.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for loading and managing
 * OpenGL shaders using external GLSL documents.
 * @date 2024-09-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#ifndef LETO__FILES__EXTERNAL
#define LETO__FILES__EXTERNAL

#include <stddef.h>

#define LETO_FILE_PATH_MAX 256

void LetoReadFile(char **buffer, const char *path);
void LetoReadFileV(char **buffer, const char *path_format, ...);

void LetoReadLines(char ***lines, size_t *line_count, const char *path);
void LetoReadLinesV(char ***lines, size_t *line_count,
                    const char *path_format, ...);

#endif // LETO__FILES__EXTERNAL
