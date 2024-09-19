#include <external/shader.h>
#include <interface/renderer.h>
#include <interface/window.h>

//! temp
#include <external/meshes.h>

unsigned int basic_shader;

bool display_init(uint16_t window_width, uint16_t window_height)
{
    basic_shader = LetoLoadShader("basic");
    if (basic_shader == 0) return false;

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
    leto_mesh_t *mesh = LetoLoadMesh("cube", wavefront);
    if (mesh == NULL) return -1;
    free(mesh);

    leto_window_info_t window_info;
    LetoCreateWindow(&window_info, "Leto");

    LetoSetRenderFunction(display);
    LetoSetRenderInitFunction(display_init);

    if (!LetoRunRenderLoop(&window_info)) return -1;
    LetoUnloadShader(basic_shader);

    LetoDestroyWindow(&window_info);
    return 0;
}
