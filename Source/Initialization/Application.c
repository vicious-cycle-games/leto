#include "Application.h"
#include "Window.h"
#include <Diagnostic/Version.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>

static leto_boolean_t initialized = leto_false_t;

static leto_boolean_t first_mouse = leto_true_t;

static void MouseCallback_(GLFWwindow *window, double x, double y)
{
    leto_application_t *application =
        (leto_application_t *)glfwGetWindowUserPointer(window);

    if (first_mouse == leto_true_t)
    {
        application->camera->last_x = (float)x;
        application->camera->last_y = (float)y;
        first_mouse = leto_false_t;
    }

    LetoMoveCameraOrientation(application->camera, (float)x, (float)y);
}

static void ProcessKeyboard_(GLFWwindow *window)
{
    leto_application_t *application =
        (leto_application_t *)glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               forward);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               backwards);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               left);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               right);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               up);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        LetoMoveCameraPosition(application->camera, application->deltatime,
                               down);
}

leto_application_t *LetoInitApplication(leto_boolean_t paused,
                                        leto_boolean_t muted)
{
    if (initialized == leto_true_t) return NULL;

    leto_application_t *application;
    LETO_ALLOC_OR_FAIL(application, sizeof(leto_application_t));
    application->flags.paused = paused;
    application->flags.muted = muted;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int glfw_initialized = glfwInit();
    if (glfw_initialized == GLFW_FALSE)
        LetoReportError(leto_true_t, failed_glfw_init, LETO_FILE_CONTEXT);

    application->window = CreateWindow("Leto | v" LETO_VERSION_STRING);
    if (application->window == NULL) return NULL;
    glfwSetInputMode(application->window, GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(application->window, application);
    glfwSetCursorPosCallback(application->window, MouseCallback_);

    int glad_initialized = gladLoadGL(glfwGetProcAddress);
    if (glad_initialized == 0)
        LetoReportError(leto_true_t, failed_glad_init, LETO_FILE_CONTEXT);

    application->camera = LetoCreateCamera(45.0f, 2.5f, 0.1f);

    initialized = leto_true_t;
    return application;
}

void LetoTerminateApplication(leto_application_t *application)
{
    if (initialized == leto_false_t || application == NULL) return;

    DestroyWindow(application->window);
    glfwTerminate();
    free(application);
}

leto_boolean_t LetoRunApplication(leto_application_t *application)
{
    if (initialized != leto_true_t || application == NULL)
        return leto_false_t;

    int width, height;
    glfwGetWindowSize(application->window, &width, &height);

    // The initialization function is optional but the actual display
    // function is mandatory.
    if (application->display_functions.init != NULL &&
        application->display_functions.init(
            width, height, application->display_functions.init_ptr) ==
            leto_false_t)
        return leto_false_t;

    if (application->display_functions.run == NULL)
    {
        LetoReportError(leto_false_t, no_display_func, LETO_FILE_CONTEXT);
        return leto_false_t;
    }

    float last_frame = 0.0f;
    while (!glfwWindowShouldClose(application->window))
    {
        float current_frame = (float)glfwGetTime();
        application->deltatime = current_frame - last_frame;
        last_frame = current_frame;

        ProcessKeyboard_(application->window);

        application->display_functions.run(
            width, height, application->display_functions.run_ptr);

        glfwPollEvents();
        glfwSwapBuffers(application->window);
    }

    if (application->display_functions.kill != NULL)
        application->display_functions.kill();

    return leto_true_t;
}

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_func_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.init = func;
    application->display_functions.init_ptr = ptr;
}

void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_func_t func)
{
    if (application == NULL) return;
    application->display_functions.kill = func;
}

void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_func_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.run = func;
    application->display_functions.run_ptr = ptr;
}
