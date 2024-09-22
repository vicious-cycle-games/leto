#include <external/shader.h>
#include <interface/renderer.h>
#include <interface/window.h>

//! temp
#include <external/models/loading.h>

unsigned int basic_shader;
unsigned int VAO, VBO;
leto_model_t *cube = NULL;

bool display_init(uint16_t window_width, uint16_t window_height)
{
    basic_shader = LetoLoadShader("basic");
    if (basic_shader == 0) return false;

    cube = LetoLoadModel("cube.obj", wavefront);
    if (cube == NULL) return false;
    // free(cube);

    glViewport(0, 0, window_width, window_height);
    return true;
}

void display(size_t argc, void **argv)
{
    (void)argc; // Unused for now.
    (void)argv;

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    LetoSwapWindowBuffers();
}

int main(void)
{

    leto_window_info_t window_info;
    LetoCreateWindow(&window_info, "Leto");

    LetoSetRenderFunction(display);
    LetoSetRenderInitFunction(display_init);

    if (!LetoRunRenderLoop(&window_info)) return -1;
    LetoUnloadShader(basic_shader);

    LetoDestroyWindow(&window_info);
    return 0;
}
