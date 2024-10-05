#ifndef LETO__APPLICATION_H
#define LETO__APPLICATION_H

#include <GLAD2/gl.h>
#include <GLFW/glfw3.h>
#include <Utilities/Types.h>

typedef struct leto_application
{
    struct leto_global_flags
    {
        leto_boolean_t paused : 1;
        leto_boolean_t muted : 1;
    } flags;
    GLFWwindow *window;
} leto_application_t;

leto_application_t *LetoInitApplication(leto_boolean_t paused,
                                        leto_boolean_t muted);
void LetoTerminateApplication(leto_application_t *application);
leto_boolean_t LetoRunApplication(leto_application_t *application);

#endif // LETO__APPLICATION_H
