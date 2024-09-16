#include "renderer.h"
#include "window.h"
#include <glfw3.h>

static render_function_t render_function = NULL;
static size_t render_arg_count = 0;
static void **render_arg_array = NULL;

void SetRenderFunction(render_function_t func) { render_function = func; }

void SetRenderData(size_t argc, void **argv)
{
    if (argc == 0 || argv == NULL) return;
    render_arg_count = argc;
    render_arg_array = argv;
}

void RunRenderLoop(void)
{
    if (render_function == NULL) return;

    while (LetoGetWindowRunState())
    {
        glfwPollEvents();
        render_function(render_arg_count, render_arg_array);
    }
}
