#ifndef LETO__APPLICATION_H
#define LETO__APPLICATION_H

#include <GLAD2/gl.h>
#include <GLFW/glfw3.h>
#include <Utilities/Types.h>

typedef void (*display_run_func_t)(int window_width, int window_height);
typedef leto_boolean_t (*display_init_func_t)(int window_width,
                                              int window_height);

typedef struct leto_application
{
    struct leto_global_flags
    {
        leto_boolean_t paused : 1;
        leto_boolean_t muted : 1;
    } flags;
    GLFWwindow *window;
    display_init_func_t display_init_function;
    display_run_func_t display_function;
} leto_application_t;

leto_application_t *LetoInitApplication(leto_boolean_t paused,
                                        leto_boolean_t muted);
void LetoTerminateApplication(leto_application_t *application);
leto_boolean_t LetoRunApplication(leto_application_t *application);

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_func_t func);
void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_func_t func);

#endif // LETO__APPLICATION_H
