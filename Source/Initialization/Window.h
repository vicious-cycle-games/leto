/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Defines the public interface for windowing, a single layer over
 * GLFW, the windowing library Leto uses.
 * @date 2024-10-07
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__WINDOW_H
#define LETO__WINDOW_H

// GLFW functions and structures.
#include <GLFW/glfw3.h>
// Standard boolean definitions.
#include <stdbool.h>

/**
 * @brief A collection of data about the window object. The "width" and
 * "height" values are automatically updated when the window is
 * resized. This struct is not particularly useful unless connected
 * directly to a greater application structure.
 */
typedef struct leto_window
{
    /**
     * @brief The actual window object.
     */
    GLFWwindow *_;
    /**
     * @brief The width of the window in screen coordinates.
     */
    int width;
    /**
     * @brief The height of the window in screen coordinates.
     */
    int height;
    /**
     * @brief The title of the window.
     */
    char *title;
} leto_window_t;

/**
 * CreateWindow
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Initialize the given window object, along with its context and
 * all flags.
 *
 * @param window The window object in which to create the new window. If
 * this is NULL, the function returns false.
 * @param title The title of the window object. If this is NULL, the
 * window's title will be "unset".
 * @return bool -- True for success, false if any failure occurred.
 */
bool LetoCreateWindow(leto_window_t *window, const char *title);

/**
 * DestroyWindow
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Deallocate all memory to do with the given window.
 *
 * @param window The window to destroy. If this or its child
 * GLFWwindow are NULL, this function returns without doing anything.
 * @return void -- Nothing.
 */
void LetoDestroyWindow(leto_window_t *window);

/**
 * ChangeWindowTitle
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Change the title of the given window. This creates a unique
 * pointer from the title string, so there is no obligation to keep the
 * given title pointer alive after this function is called.
 *
 * @param window The window whose title to change. If this or its child
 * GLFWwindow are NULL, this function returns without doing anything.
 * @param title The title string to be set. No changes are made to this
 * string internally, it's simply copied into the necessary memory slot.
 * @return void -- Nothing.
 */
void LetoChangeWindowTitle(leto_window_t *window, const char *title);

#endif // LETO__WINDOW_H
