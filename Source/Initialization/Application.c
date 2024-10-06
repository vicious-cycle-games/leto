#include "Application.h"
#include "Window.h"
#include <Diagnostic/Version.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>

static bool initialized = false;
static bool first_mouse = true;

static void MouseCallback_(GLFWwindow *window, double x, double y)
{
    leto_application_t *application =
        (leto_application_t *)glfwGetWindowUserPointer(window);

    if (first_mouse == true)
    {
        application->camera->last_x = (float)x;
        application->camera->last_y = (float)y;
        first_mouse = false;
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

leto_application_t *LetoInitApplication(bool paused, bool muted)
{
    if (initialized == true) return NULL;

    leto_application_t *application;
    LETO_ALLOC_OR_FAIL(application, sizeof(leto_application_t));
    application->flags.paused = paused;
    application->flags.muted = muted;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int glfw_initialized = glfwInit();
    if (glfw_initialized == GLFW_FALSE)
        LetoReportError(true, failed_glfw_init, LETO_FILE_CONTEXT);

    application->window = CreateWindow("Leto | v" LETO_VERSION_STRING);
    if (application->window == NULL) return NULL;
    glfwSetInputMode(application->window, GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(application->window, application);
    glfwSetCursorPosCallback(application->window, MouseCallback_);

    int glad_initialized = gladLoadGL(glfwGetProcAddress);
    if (glad_initialized == 0)
        LetoReportError(true, failed_glad_init, LETO_FILE_CONTEXT);

    application->camera = LetoCreateCamera(45.0f, 2.5f, 0.1f);

    initialized = true;
    return application;
}

void LetoTerminateApplication(leto_application_t *application)
{
    if (initialized == false || application == NULL) return;

    DestroyWindow(application->window);
    glfwTerminate();
    free(application);
}

bool LetoRunApplication(leto_application_t *application)
{
    if (initialized != true || application == NULL) return false;

    int width, height;
    glfwGetWindowSize(application->window, &width, &height);

    // The initialization function is optional but the actual display
    // function is mandatory.
    if (application->display_functions.init != NULL &&
        application->display_functions.init(
            width, height, application->display_functions.init_ptr) ==
            false)
        return false;

    if (application->display_functions.run == NULL)
    {
        LetoReportError(false, no_display_func, LETO_FILE_CONTEXT);
        return false;
    }

    float last_frame = 0.0f;
    while (!glfwWindowShouldClose(application->window))
    {
        float current_frame = (float)glfwGetTime();
        application->deltatime = current_frame - last_frame;
        last_frame = current_frame;

        ProcessKeyboard_(application->window);

        application->display_functions.run(
            application->deltatime,
            application->display_functions.run_ptr);

        glfwPollEvents();
        glfwSwapBuffers(application->window);
    }

    if (application->display_functions.kill != NULL)
        application->display_functions.kill();

    return true;
}

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.init = func;
    application->display_functions.init_ptr = ptr;
}

void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_t func)
{
    if (application == NULL) return;
    application->display_functions.kill = func;
}

void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_t func, void *ptr)
{
    if (application == NULL) return;
    application->display_functions.run = func;
    application->display_functions.run_ptr = ptr;
}
