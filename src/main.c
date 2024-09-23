#include <external/shader.h>
#include <interface/renderer.h>
#include <interface/window.h>

//! temp
#include <affine.h>
#include <cam.h>
#include <external/models/loading.h>
#include <external/models/rendering.h>
#include <mat4.h>

unsigned int basic_shader;
unsigned int VAO, VBO;
leto_model_t *cube = NULL;

mat4 view = GLM_MAT4_IDENTITY_INIT, projection = GLM_MAT4_IDENTITY_INIT;

bool display_init(uint16_t window_width, uint16_t window_height)
{
    basic_shader = LetoLoadShader("basic");
    if (basic_shader == 0) return false;

    cube = LetoLoadModel("cube.obj", wavefront);
    if (cube == NULL) return false;
    // free(cube);

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0, 0, window_width, window_height);

    glUseProgram(basic_shader);
    glm_perspective(glm_rad(45.0f),
                    (float)window_width / (float)window_height, 0.1f,
                    100.0f, projection);

    glm_translate(view, (vec3){0.0f, 0.0f, -5.0f});
    glUniformMatrix4fv(glGetUniformLocation(basic_shader, "projection"), 1,
                       GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(basic_shader, "view"), 1,
                       GL_FALSE, &view[0][0]);

    return true;
}

void display(size_t argc, void **argv)
{
    (void)argc; // Unused for now.
    (void)argv;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    LetoRenderModel(cube, basic_shader);

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
