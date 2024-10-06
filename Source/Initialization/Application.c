#include "Application.h"
#include "Window.h"
#include <Diagnostic/Version.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>

static leto_boolean_t initialized = leto_false_t;

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
#if defined(LETO_APPLE)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    int glfw_initialized = glfwInit();
    if (glfw_initialized == GLFW_FALSE)
        LetoReportError(leto_true_t, failed_glfw_init, LETO_FILE_CONTEXT);

    application->window = CreateWindow("Leto | v" LETO_VERSION_STRING);
    if (application->window == NULL) return NULL;

    int glad_initialized = gladLoadGL(glfwGetProcAddress);
    if (glad_initialized == 0)
        LetoReportError(leto_true_t, failed_glad_init, LETO_FILE_CONTEXT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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
    if (application->display_init_function != NULL)
        application->display_init_function(width, height);

    if (application->display_function == NULL)
    {
        LetoReportError(leto_false_t, no_display_func, LETO_FILE_CONTEXT);
        return leto_false_t;
    }

    while (!glfwWindowShouldClose(application->window))
    {
        application->display_function(width, height);

        glfwPollEvents();
        glfwSwapBuffers(application->window);
    }

    if (application->display_kill_function != NULL)
        application->display_kill_function();

    return leto_true_t;
}

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_func_t func)
{
    if (application == NULL) return;
    application->display_init_function = func;
}

void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_func_t func)
{
    if (application == NULL) return;
    application->display_kill_function = func;
}

void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_func_t func)
{
    if (application == NULL) return;
    application->display_function = func;
}
