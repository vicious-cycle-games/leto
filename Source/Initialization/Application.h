#ifndef LETO__APPLICATION_H
#define LETO__APPLICATION_H

#include <GLAD2/gl.h>
#include <GLFW/glfw3.h>
#include <Rendering/Camera.h>
#include <Utilities/Types.h>

typedef leto_boolean_t (*display_init_func_t)(int window_width,
                                              int window_height,
                                              void *ptr);
typedef void (*display_kill_func_t)(void);
typedef void (*display_run_func_t)(int window_width, int window_height,
                                   void *ptr);

typedef struct leto_application
{
    struct leto_global_flags
    {
        leto_boolean_t paused : 1;
        leto_boolean_t muted : 1;
    } flags;
    GLFWwindow *window;
    leto_camera_t *camera;
    struct leto_display_functions
    {
        display_init_func_t init;
        display_kill_func_t kill;
        display_run_func_t run;
        void *init_ptr;
        void *run_ptr;
    } display_functions;
    float deltatime;
} leto_application_t;

leto_application_t *LetoInitApplication(leto_boolean_t paused,
                                        leto_boolean_t muted);
void LetoTerminateApplication(leto_application_t *application);
leto_boolean_t LetoRunApplication(leto_application_t *application);

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_func_t func, void *ptr);
void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_func_t func);
void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_func_t func, void *ptr);

#endif // LETO__APPLICATION_H
