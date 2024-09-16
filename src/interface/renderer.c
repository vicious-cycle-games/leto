#include "renderer.h"
#include "window.h"
#include <glfw3.h>

static render_function_t render_function = NULL;
static render_init_function_t render_init_function = NULL;
static size_t render_arg_count = 0;
static void **render_arg_array = NULL;

void LetoSetRenderFunction(render_function_t func)
{
    render_function = func;
}

void LetoSetRenderInitFunction(render_init_function_t func)
{
    render_init_function = func;
}

void LetoSetRenderData(size_t argc, void **argv)
{
    if (argc == 0 || argv == NULL) return;
    render_arg_count = argc;
    render_arg_array = argv;
}

bool LetoRunRenderLoop(const leto_window_info_t *info)
{
    if (render_function == NULL || render_init_function == NULL)
        return false;

    if (!render_init_function(info->width, info->height)) return false;
    while (LetoGetWindowRunState())
    {
        glfwPollEvents();
        render_function(render_arg_count, render_arg_array);
    }

    return true;
}
