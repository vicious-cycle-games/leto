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

// The engine's windowing interface.
#include <Initialization/Window.h>
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
typedef void (*display_kill_t)(void *ptr);

/**
 * @brief Defines the blueprint for the display run function, which is
 * triggered each time a new frame should be rendered. This function is
 * passed the current deltatime (the time it took to render the last frame)
 * and a user-defined pointer (or NULL should nothing be provided).
 */
typedef void (*display_run_t)(float deltatime, void *ptr);

/**
 * @brief The global application structure of Leto. This contains all
 * global objects, like the camera, window, and flag collection. This  only
 * contains objects strictly necessary to be global, everything else is
 * within getter functions or the like.
 */
typedef struct leto_application
{
    /**
     * @brief A set of global flags deciding certain things about game
     * control, like accessibility settings, muting, pause state, etc. Each
     * member of this struct is 1 bit long.
     */
    struct leto_global_flags
    {
        /**
         * @brief A flag specifying if the game is paused or not. This does
         * not mean the game will stall, this simply means all main
         * processes should pause, and a secondary thread should spawn in
         * order to handle pause menu nonsense.
         */
        bool paused : 1;
        /**
         * @brief A flag specifying if the game is muted or not. This flag
         * being "true" means all audio processes should be killed, and
         * when turned back to "false" they will be @b re-initialized. This
         * means that audio methods should be separate from display logic,
         * because display initialization methods are only called once.
         */
        bool muted : 1;
        /**
         * @brief A flag specifying if the game is in "developer mode",
         * meaning cheats are enabled and the console is given to the
         * player. This is an option set from the main menu, and there are
         * no achievements earned in this mode.
         */
        bool developer : 1;
    } flags;
    /**
     * @brief A collection of functions (and their user pointers) that
     * pertain to displaying content. Each is in the sturcture of
     * container --> (func, ptr).
     */
    struct leto_display_functions
    {
        /**
         * @brief The initialization function for the display. In this
         * should be loading logic and maybe some preliminary checks to
         * make sure logic is working correctly.
         */
        struct leto_display_init
        {
            /**
             * @brief The function to be called.
             */
            display_init_t _;
            /**
             * @brief The user pointer to be passed into the function by
             * the display routine.
             */
            void *ptr;
        } init;
        /**
         * @brief The active function for the display, or the function
         * that's called each frame. This should contain the actual
         * rendering logic, and if it fails irrepairably should simply kill
         * the application outright.
         */
        struct leto_display_run
        {
            /**
             * @brief The function to be called.
             */
            display_run_t _;
            /**
             * @brief The user pointer to be passed into the function by
             * the display routine.
             */
            void *ptr;
        } run;
        /**
         * @brief The function to be called when the display quits. This is
         * @b not called if we run into an error. This function should be
         * used to de-load assets and content, and shouldn't be able to
         * fail under normal circumstances.
         */
        struct leto_display_kill
        {
            /**
             * @brief The function to be called.
             */
            display_kill_t _;
            /**
             * @brief The user pointer to be passed into the function by
             * the display routine.
             */
            void *ptr;
        } kill;
    } display_functions;
    /**
     * @brief Some benchmark data polled by the display routine, like FPS
     * and deltatime.
     */
    struct leto_render_benchmarks
    {
        /**
         * @brief The time it took to render the last frame. We use this to
         * normalize some rendering stuff, but this value should @b not be
         * used to normalize updating things, as that will make the game
         * run faster at a higher FPS.
         */
        float deltatime;
        /**
         * @brief The amount of frames we render each second. This is only
         * stored so we can display it when the game is in developer mode.
         */
        float fps;
    } render_benchmarks;
    /**
     * @brief The window of the application.
     */
    leto_window_t window;
    /**
     * @brief The camera object of the application. This defines the FOV,
     * viewport, etc.
     */
    leto_camera_t *camera;
} leto_application_t;

/**
 * InitApplication
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Initialize the application and all of its states. This does not
 * set the display functions, nor does it kick off the display cycle. Note
 * that this function automatically sets the FOV at 45.0f.
 *
 * @param paused A boolean value representing the pause state.
 * @param muted A boolean value representing the mute state.
 * @param devmode A boolean value representing the enable state of devmode.
 * @return leto_application_t* -- A pointer to the newly created
 * application structure.
 */
leto_application_t *LetoInitApplication(bool paused, bool muted,
                                        bool devmode);

/**
 * TerminateApplication
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Terminate all data having to do with the application. Past this
 * function, there should not be a single byte of memory allocated to the
 * application left, besides the things we cannot free like certain pieces
 * of OpenGL context data.
 *
 * @param application The application to check. If this is NULL, the
 * function simply returns without doing anything.
 * @return void -- Nothing.
 */
void LetoTerminateApplication(leto_application_t *application);

/**
 * RunApplication
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Run the display loop of the application. Note that there must at
 * least be a bound display run function at the time of this function's
 * call. Without that, it will return false and do nothing. When this
 * function returns, take it as a sign to close the application, regardless
 * of error state.
 *
 * @param application The application to check. If this is NULL, the
 * function simply returns without doing anything.
 * @return bool -- True for success, false for failure. All failures are
 * reported to @ref errid.
 */
bool LetoRunApplication(leto_application_t *application);

/**
 * BindDisplayInitFunc
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Bind the function that will be called on the initialization of
 * the display. This is an optional function, but should be used to load
 * resources and check to make certain logic is in order.
 *
 * @param application The application to check. If this is NULL, the
 * function simply returns without doing anything.
 * @param func The function to call.
 * @param ptr The user pointer for the function, passed to it when called.
 * This can be anything.
 * @return void -- Nothing.
 */
void LetoBindDisplayInitFunc(leto_application_t *application,
                             display_init_t func, void *ptr);

/**
 * BindDisplayRunFunc
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Bind the function that will be called every time a frame should
 * be rendered. This function is mandatory for the running of the
 * application's display loop, and should simply render and update screen
 * content.
 *
 * @param application The application to check. If this is NULL, the
 * function simply returns without doing anything.
 * @param func The function to call.
 * @param ptr The user pointer for the function, passed to it when called.
 * This can be anything.
 * @return void -- Nothing.
 */
void LetoBindDisplayRunFunc(leto_application_t *application,
                            display_run_t func, void *ptr);

/**
 * BindDisplayKillFunc
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Bind the function that will be called when the display loop ends.
 * This function is optional, and will @b not be called in case of an
 * error. This function should free all data, meaning nothing allocated to
 * do with resources and such should still be allocated after this function
 * call
 *
 * @param application The application to check. If this is NULL, the
 * function simply returns without doing anything.
 * @param func The function to call.
 * @param ptr The user pointer for the function, passed to it when called.
 * This can be anything.
 * @return void -- Nothing.
 */
void LetoBindDisplayKillFunc(leto_application_t *application,
                             display_kill_t func, void *ptr);

#endif // LETO__APPLICATION_H
