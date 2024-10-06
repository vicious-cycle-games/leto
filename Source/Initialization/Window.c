#include "Window.h"
#include <Diagnostic/Platform.h>
#include <Output/Errors.h>

GLFWwindow *CreateWindow(const char *title)
{
    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL)
    {
        LetoReportError(false, failed_monitor_get, LETO_FILE_CONTEXT);
        return NULL;
    }
    const GLFWvidmode *resolution = glfwGetVideoMode(primary_monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

// Figure out some details for Linux windows.
#if defined(LETO_WAYLAND)
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, title);
#elif defined(LETO_X11)
    glfwWindowHintString(GLFW_X11_CLASS_NAME, title);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, title);
#endif

    GLFWwindow *created_window = glfwCreateWindow(
        resolution->width, resolution->height, title, NULL, NULL);
    if (created_window == NULL)
        LetoReportError(true, failed_window_create, LETO_FILE_CONTEXT);

    glfwMakeContextCurrent(created_window);
    glfwMaximizeWindow(created_window);
    glfwSetWindowPos(created_window, 0, 0);

    return created_window;
}

void DestroyWindow(GLFWwindow *window)
{
    if (window == NULL) return;
    glfwDestroyWindow(window);
}
