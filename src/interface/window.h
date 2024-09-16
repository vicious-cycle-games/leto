/**
 * @file window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for the windowing section
 * of Leto. This includes functions to deal with all aspects of windowing;
 * state management, getters & setters, and more.
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

#ifndef LETO__WINDOW__INTERFACE
#define LETO__WINDOW__INTERFACE

// Standard fixed-width integers.
#include <stdint.h>
// Standard boolean macros.
#include <stdbool.h>

typedef struct
{
    uint16_t width;
    uint16_t height;
    const char *title;
} leto_window_info_t;

#define LETO_WINDOW_INFO_NULL                                             \
    (leto_window_info_t) { 0, 0, NULL }

#define LETO_WINDOW_TITLE_MAX 128

void LetoCreateWindow(leto_window_info_t *info_storage, const char *title);
void LetoDestroyWindow(leto_window_info_t *info_storage);
bool LetoGetWindowRunState(void);
void LetoSwapWindowBuffers(void);

#endif // LETO__WINDOW__INTERFACE
