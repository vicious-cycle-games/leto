#include <interface/renderer.h>
#include <interface/window.h>

void display(size_t argc, void **argv)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    LetoSwapWindowBuffers();
}

int main(void)
{
    leto_window_info_t window_info;
    LetoCreateWindow(&window_info, "Leto");

    SetRenderFunction(display);
    RunRenderLoop();

    LetoDestroyWindow(&window_info);
    return 0;
}
