#include "Window.h"
#include <Diagnostic/Platform.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>
#include <string.h>

bool LetoCreateWindow(leto_window_t *window, const char *title)
{
    if (window == NULL) return false;

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
                                 "unset", NULL, NULL);
    if (window->_ == NULL)
        LetoReportError(true, failed_window_create, LETO_FILE_CONTEXT);
    glfwMakeContextCurrent(window->_);

    window->width = resolution->width;
    window->height = resolution->height;
    if (title != NULL) LetoChangeWindowTitle(window, title);

    glfwSetInputMode(window->_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowPos(window->_, 0, 0);
    glfwMaximizeWindow(window->_);

    return true;
}

void LetoDestroyWindow(leto_window_t *window)
{
    if (window == NULL || window->_ == NULL) return;
    glfwDestroyWindow(window->_);

    free((char *)window->title);
    window->title = NULL;
    window->width = 0;
    window->height = 0;
}

void LetoChangeWindowTitle(leto_window_t *window, const char *title)
{
    if (window == NULL || window->_ == NULL) return;

    free((char *)window->title);
    window->title = strdup(title);

    glfwSetWindowTitle(window->_, window->title);
}
