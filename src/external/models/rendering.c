#include "rendering.h"
#include <affine.h>
#include <gl.h>

void LetoRenderModel(leto_model_t *model, unsigned int shader)
{
    glUseProgram(shader);
    for (size_t i = 0; i < model->meshes.count; i++)
    {
        leto_mesh_t *current_mesh = &model->meshes._[i];

        mat4 mod = GLM_MAT4_IDENTITY_INIT;
        glm_translate(mod, (vec3){model->position.x, model->position.y,
                                  model->position.z});
        glm_rotate(mod, model->rotation, (vec3){1.0f, 0.3f, 0.5f});
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1,
                           GL_FALSE, &mod[0][0]);

        glBindVertexArray(current_mesh->VAO);
        glDrawElements(GL_TRIANGLES, (int)current_mesh->indices.count,
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
