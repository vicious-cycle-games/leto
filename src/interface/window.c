/**
 * @file window.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the implementation of the public windowing
 * interface defined within @file window.h.
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

#include "window.h"   // Public interface parent.
#include <gl.h>       // OpenGL function pointers.
#include <glfw3.h>    // GLFW function pointers.
#include <malloc.h>   // Standard allocation functions.
#include <platform.h> // Platform macros.
#include <stdio.h>    // Standard I/O functionality.

/**
 * @brief The internal GLFW handle of the application's window. This is
 * created by @ref CreateWindow and freed + set back to NULL by @ref
 * DestroyWindow.
 */
static GLFWwindow *window_ = NULL;

static void LetoConstructTitle_(char **buffer, char *half_title)
{
    *buffer = malloc(LETO_WINDOW_TITLE_MAX);
    if (*buffer == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        *buffer = NULL;
    }
    snprintf(*buffer, LETO_WINDOW_TITLE_MAX, "%s | v" LETO_VERSION,
             half_title);
}

static void LetoSetHints_(char *title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(LETO_APPLE)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
    glfwWindowHint(GLFW_POSITION_X, 0); // Initial position for the window.
    glfwWindowHint(GLFW_POSITION_Y, 0);

// Figure out some details for Linux windows.
#if defined(LETO_WAYLAND)
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, title);
#elif defined(LETO_X11)
    glfwWindowHintString(GLFW_X11_CLASS_NAME, title);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, title);
#endif
}

void LetoCreateWindow(leto_window_info_t *info_storage, const char *title)
{
    if (window_ != NULL)
    {
        *info_storage = LETO_WINDOW_INFO_NULL;
        return;
    }

    int glfw_initialized = glfwInit();
    if (glfw_initialized == GLFW_FALSE)
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        *info_storage = LETO_WINDOW_INFO_NULL;
        return;
    }

    // Get the primary monitor's resolution so we can size our window
    // properly.
    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *primary_resolution =
        glfwGetVideoMode(primary_monitor);

    char *full_title = NULL;
    LetoConstructTitle_(&full_title, (char *)title);
    if (full_title == NULL)
    {
        *info_storage = LETO_WINDOW_INFO_NULL;
        return;
    }

    LetoSetHints_(full_title);
    window_ = glfwCreateWindow(primary_resolution->width,
                               primary_resolution->height, full_title,
                               NULL, NULL);
    if (window_ == NULL)
    {
        fprintf(stderr, "Failed to create window.\n");
        free(full_title);
        *info_storage = LETO_WINDOW_INFO_NULL;
        return;
    }
    // Make the OpenGL context of the created window current on the main
    // thread.
    glfwMakeContextCurrent(window_);

    int glad_initialized = gladLoadGL(glfwGetProcAddress);
    if (glad_initialized == 0)
    {
        fprintf(stderr, "Failed to initialize GLAD.\n");
        free(full_title);
        *info_storage = LETO_WINDOW_INFO_NULL;
        return;
    }
    glViewport(0, 0, primary_resolution->width,
               primary_resolution->height);

    info_storage->width = (uint16_t)primary_resolution->width;
    info_storage->height = (uint16_t)primary_resolution->height;
    info_storage->title = full_title;
}

void LetoDestroyWindow(leto_window_info_t *info_storage)
{
    if (window_ == NULL) return;
    // Resolve all resources allocated to the window object.
    glfwDestroyWindow(window_);
    window_ = NULL;
    glfwTerminate(); // Free all other GLFW resources.

    free((void *)info_storage->title);
    info_storage->width = 0;
    info_storage->height = 0;
    info_storage->title = NULL;
}

bool LetoGetWindowRunState(void)
{
    return !glfwWindowShouldClose(window_);
}

void LetoSwapWindowBuffers(void) { glfwSwapBuffers(window_); }
