/**
 * @file Application.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Implements the base of Leto's engine. This includes
 * initialization and termination functions, callbacks, and some more.
 * @implements Application.h
 * @date 2024-10-06
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Application.h"   // Public interface parent
#include "Window.h"        // Engine windowing functions
#include <Output/Errors.h> // Error reporting

#include <Diagnostic/Version.h> // Version information
#include <Utilities/Macros.h>   // Utility macros

/**
 * FramebufferCallback
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief The function called each time the window is resized. This is
 * mostly provided so we can scale content appropriately.
 *
 * @param window The window that was resized.
 * @param width The new width of the window in screen coordinates.
 * @param height The new height of the window in screen coordinates.
 * @return void -- Nothing.
 */
static void FramebufferCallback_(GLFWwindow *window, int width, int height)
{
    leto_application_t *application =
        (leto_application_t *)glfwGetWindowUserPointer(window);
    application->window.width = width;
    application->window.height = height;

    glViewport(0, 0, width, height);
}

/**
 * MouseCallback
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief The function called each time the user moves their cursor. This
 * is mainly a wrapper around moving the camera.
 *
 * @param window The window on which the mouse moved.
 * @param x The X coordinate of the mouse.
 * @param y The Y coordinate of the mouse.
 * @return void -- Nothing.
 */
static void MouseCallback_(GLFWwindow *window, double x, double y)
{
    leto_application_t *application =
        (leto_application_t *)glfwGetWindowUserPointer(window);

    // We've gotta initialize this value to prevent snapping when the
    // camera is first moved.
    static bool mouse_moved;
    if (mouse_moved == false)
    {
        application->camera->last_x = (float)x;
        application->camera->last_y = (float)y;
        mouse_moved = true;
    }

    LetoMoveCameraOrientation(application->camera, (float)x, (float)y);
}

leto_application_t *LetoInitApplication(bool paused, bool muted,
                                        bool devmode)
{
    leto_application_t *application;
    LETO_ALLOC_OR_FAIL(application, sizeof(leto_application_t));

    application->flags.paused = paused;
    application->flags.muted = muted;
    application->flags.developer = devmode;

    // OpenGL Core v4.6.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int glfw_initialized = glfwInit();
    if (glfw_initialized == GLFW_FALSE)
        LetoReportError(true, failed_glfw_init, LETO_FILE_CONTEXT);

    application->window._ =
        LetoCreateWindow("Leto | v" LETO_VERSION_STRING);
    if (application->window._ == NULL) return NULL;
    glfwSetWindowUserPointer(application->window._, application);
    glfwGetWindowSize(application->window._, &application->window.width,
                      &application->window.height);

    // Disable the cursor, locking and hiding its sprite.
    glfwSetInputMode(application->window._, GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(application->window._,
                                   FramebufferCallback_);
    glfwSetCursorPosCallback(application->window._, MouseCallback_);

    int glad_initialized = gladLoadGL(glfwGetProcAddress);
    if (glad_initialized == 0)
        LetoReportError(true, failed_glad_init, LETO_FILE_CONTEXT);

    // Initialize the camera with an FOV of 45, a movement speed of 2.5,
    // and a sensitivity of 0.1.
    application->camera = LetoCreateCamera(45.0f, 2.5f, 0.1f);

    return application;
}

void LetoTerminateApplication(leto_application_t *application)
{
    if (application == NULL) return;

    LetoDestroyWindow(application->window._);
    LetoDestroyCamera(application->camera);

    glfwTerminate();
    free(application);
}

bool LetoRunApplication(leto_application_t *application)
{
    if (application == NULL) return false;

    // If the initialization function 1. exists and 2. failed, return
    // error.
    if (application->display_functions.init._ != NULL &&
        application->display_functions.init._(
            application->window.width, application->window.height,
            application->display_functions.init.ptr) == false)
        return false;

    if (application->display_functions.run._ == NULL)
    {
        LetoReportError(false, no_display_func, LETO_FILE_CONTEXT);
        return false;
    }

    float last_frame = 0.0f;
    while (!glfwWindowShouldClose(application->window._))
    {
        // Recalculate the deltatime every frame.
        float current_frame = (float)glfwGetTime();
        application->render_benchmarks.deltatime =
            current_frame - last_frame;
        last_frame = current_frame;

        application->display_functions.run._(
            application->render_benchmarks.deltatime,
            application->display_functions.run.ptr);

        glfwPollEvents();
        glfwSwapBuffers(application->window._);
    }

    if (application->display_functions.kill._ != NULL)
        application->display_functions.kill._(
            application->display_functions.kill.ptr);

    return true;
}

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.init._ = func;
    application->display_functions.init.ptr = ptr;
}

void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.kill._ = func;
    application->display_functions.kill.ptr = ptr;
}

void LetoBindDisplayRunFunc(leto_application_t *application,
                            display_run_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.run._ = func;
    application->display_functions.run.ptr = ptr;
}
