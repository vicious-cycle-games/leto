#include <CGLM/affine.h>
#include <Initialization/Application.h>
#include <Input/Shaders.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int basic_shader;
unsigned int vao, vbo;

float vertices[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f,
                    0.0f,  0.0f,  1.0f, 0.0f};

mat4 mod = GLM_MAT4_IDENTITY_INIT;

leto_boolean_t init(int width, int height, void *ptr)
{
    basic_shader = LetoLoadShader("basic");
    if (basic_shader == 0) return leto_false_t;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // normals
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //                       (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // // texture coord attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //                       (void *)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    glUseProgram(basic_shader);
    glUniformMatrix4fv(glGetUniformLocation(basic_shader, "model"), 1,
                       GL_FALSE, &mod[0][0]);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glViewport(0, 0, width, height);
    return leto_true_t;
}

void run(int width, int height, void *ptr)
{
    leto_application_t *application = (leto_application_t *)ptr;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    LetoSetProjection(basic_shader, 45.0f, (float)width / (float)height,
                      0.1f, 100.0f);
    LetoSetCameraMatrix(application->camera, basic_shader);

    glUseProgram(basic_shader);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 9);
}

void dkill(void) { LetoUnloadShader(basic_shader); }

int main(void)
{
    leto_application_t *leto =
        LetoInitApplication(leto_false_t, leto_false_t);
    if (leto == NULL) exit(EXIT_FAILURE);

    LetoBindDisplayInitFunc(leto, init, leto);
    LetoBindDisplayKillFunc(leto, dkill);
    LetoBindDisplayFunc(leto, run, leto);

    LetoRunApplication(leto);

    LetoTerminateApplication(leto);
}
