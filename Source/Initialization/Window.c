/**
 * @file Window.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Implements the layer of abstraction Leto puts between the game
 * and GLFW. This includes creation, deletion, and some limited setters.
 * @implements Window.h
 * @date 2024-10-07
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Window.h"        // Public interface parent
#include <Output/Errors.h> // Error reporting

#include <Diagnostic/Platform.h> // Platform macros
#include <Utilities/Macros.h>    // Utility preprocessor macros

#include <string.h> // Standard string utilities

bool LetoCreateWindow(leto_window_t *window, const char *title)
{
    if (window == NULL) return false;

    // Get the GLFW representation of the user's "primary" monitor (really
    // just the first one GLFW finds).
    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL)
    {
        LetoReportError(false, failed_monitor_get, LETO_FILE_CONTEXT);
        return false;
    }
    const GLFWvidmode *resolution = glfwGetVideoMode(primary_monitor);

    // The window shall be neither resizable nor decorated in any way.
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

// Figure out some details for Linux windows.
#if defined(LETO_WAYLAND)
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, title);
#elif defined(LETO_X11)
    glfwWindowHintString(GLFW_X11_CLASS_NAME, title);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, title);
#endif

    window->_ = glfwCreateWindow(resolution->width, resolution->height,
                                 "unset", primary_monitor, NULL);
    if (window->_ == NULL)
        LetoReportError(true, failed_window_create, LETO_FILE_CONTEXT);
    // Make our graphics context current on this thread.
    glfwMakeContextCurrent(window->_);

    window->width = resolution->width;
    window->height = resolution->height;
    window->title = NULL;
    LetoChangeWindowTitle(window, title);

    // Hide and disable movement of the cursor. This ensures our cursor
    // doesn't move off-screen or hit a border and break gameplay.
    glfwSetInputMode(window->_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

void LetoDestroyWindow(leto_window_t *window)
{
    if (window == NULL || window->_ == NULL) return;
    glfwDestroyWindow(window->_);

    if (window->title != NULL) free(window->title);
    window->title = NULL;
    window->width = 0;
    window->height = 0;
}

void LetoChangeWindowTitle(leto_window_t *window, const char *title)
{
    if (window == NULL || window->_ == NULL) return;

    // Copy the string so we don't have to rely on the provided pointer's
    // lifetime.
    if (window->title != NULL) free(window->title);
    window->title = strdup(title);

    glfwSetWindowTitle(window->_, window->title);
}
