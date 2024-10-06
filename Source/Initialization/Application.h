/**
 * @file Application.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Defines the base of the engine: the application class. This is
 * the crux file that the application's entrypoint should always include,
 * as it contains all one needs to get the game running.
 * @date 2024-10-06
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__APPLICATION_H
#define LETO__APPLICATION_H

// OpenGL function pointers and data types.
#include <GLAD2/gl.h>
// GLFW functions and structures.
#include <GLFW/glfw3.h>

// The engine's camera interface.
#include <Rendering/Camera.h>

/**
 * @brief Defines the blueprint for the display initialization function,
 * which takes in the width and height of the application's window, a
 * user-defined pointer (or NULL should nothing be provided), and returns a
 * success flag.
 */
typedef bool (*display_init_t)(int window_width, int window_height,
                               void *ptr);

/**
 * @brief Defines the blueprint for the display kill function, which takes
 * in nothing and returns nothing. This function should not be able to
 * fail under normal circumstances.
 */
typedef void (*display_kill_t)(void);

/**
 * @brief Defines the blueprint for the display run function, which is
 * triggered each time a new frame should be rendered. This function is
 * passed the current deltatime (the time it took to render the last frame)
 * and a user-defined pointer (or NULL should nothing be provided).
 */
typedef void (*display_run_t)(float deltatime, void *ptr);

typedef struct leto_application
{
    struct leto_global_flags
    {
        bool paused : 1;
        bool muted : 1;
    } flags;
    GLFWwindow *window;
    leto_camera_t *camera;
    struct leto_display_functions
    {
        display_init_t init;
        display_kill_t kill;
        display_run_t run;
        void *init_ptr;
        void *run_ptr;
    } display_functions;
    float deltatime;
} leto_application_t;

leto_application_t *LetoInitApplication(bool paused, bool muted);
void LetoTerminateApplication(leto_application_t *application);
bool LetoRunApplication(leto_application_t *application);

void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_t func, void *ptr);
void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_t func);
void LetoBindDisplayFunc(leto_application_t *application,
                         display_run_t func, void *ptr);

#endif // LETO__APPLICATION_H
