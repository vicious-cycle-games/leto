/**
 * @file renderer.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for the rendering section
 * of Leto, including render functions and the main application loops.
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

#ifndef LETO__RENDERER__INTERFACE
#define LETO__RENDERER__INTERFACE

#include "window.h"
#include <gl.h>
#include <stdbool.h>
#include <stddef.h>

typedef void (*render_function_t)(size_t argc, void **argv);
typedef bool (*render_init_function_t)(uint16_t window_width,
                                       uint16_t window_height);

void LetoSetRenderFunction(render_function_t func);
void LetoSetRenderInitFunction(render_init_function_t func);
void LetoSetRenderData(size_t argc, void **argv);
bool LetoRunRenderLoop(const leto_window_info_t *info);

#endif // LETO__RENDERER__INTERFACE
