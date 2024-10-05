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

    while (!glfwWindowShouldClose(application->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(application->window);
    }

    return leto_true_t;
}
