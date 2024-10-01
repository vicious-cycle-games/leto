#include "rendering.h"
#include <affine.h>
#include <gl.h>
#include <output/logger.h>
#include <stdio.h>

static void printmesh(leto_mesh_t mesh)
{
    printf("mesh(0):\n");
    for (size_t i = 0; i < mesh.vertices.count; i++)
    {
        leto_vertex_t vertex = mesh.vertices._[i];
        printf(
            "vert(%zu): pos[%f, %f, %f], norm[%f, %f, %f], tex[%f, %f]\n",
            i, vertex.position.x, vertex.position.y, vertex.position.z,
            vertex.normal.x, vertex.normal.y, vertex.normal.z,
            vertex.texture.x, vertex.texture.y);
    }
    // exit(EXIT_SUCCESS);
}

void LetoRenderModel(leto_model_t *model, unsigned int shader)
{
    printmesh(model->meshes._[0]);

    glUseProgram(shader);
    for (size_t i = 0; i < model->meshes.count; i++)
    {
        leto_mesh_t *current_mesh = &model->meshes._[i];

        mat4 mod = GLM_MAT4_IDENTITY_INIT;
        // glm_translate(mod, (vec3){model->position.x, model->position.y,
        //                           model->position.z});
        // glm_rotate(mod, model->rotation, (vec3){1.0f, 0.3f, 0.5f});
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1,
                           GL_FALSE, &mod[0][0]);

        glBindVertexArray(current_mesh->VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
