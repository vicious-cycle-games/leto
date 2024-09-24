#include "wavefront.h"
#include <external/files.h>
#include <gl.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// A flag to tell us if the mesh has been filled at least once, to avoid
// the issue of the leading mesh declaration.
static bool mesh_filled = false;
static leto_mesh_t current_mesh;

void LetoUploadWavefrontMesh(leto_model_t *model)
{
    if (!mesh_filled) return;

    glGenVertexArrays(1, &current_mesh.VAO);

    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(current_mesh.VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is
    // sequential for all its items. The effect is that we can simply pass
    // a pointer to the struct and it translates perfectly to a glm::vec3/2
    // array which again translates to 3/2 floats which translates to a
    // byte array.
    glBufferData(GL_ARRAY_BUFFER,
                 current_mesh.vertices.count * sizeof(leto_vertex_t),
                 &current_mesh.vertices._[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 current_mesh.indices.count * sizeof(uint32_t),
                 &current_mesh.indices._[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(leto_vertex_t),
                          (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(leto_vertex_t),
                          (void *)offsetof(leto_vertex_t, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(leto_vertex_t),
                          (void *)offsetof(leto_vertex_t, texture));
    // // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                       (void *)offsetof(Vertex, Tangent));
    // // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                       (void *)offsetof(Vertex, Bitangent));
    // // ids
    // glEnableVertexAttribArray(5);
    // glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex),
    //                        (void *)offsetof(Vertex, m_BoneIDs));

    // // weights
    // glEnableVertexAttribArray(6);
    // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                       (void *)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);

    model->meshes._ = realloc(
        model->meshes._, sizeof(leto_mesh_t) * (model->meshes.count + 1));
    if (model->meshes._ == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
    model->meshes._[model->meshes.count] = current_mesh;
    model->meshes.count += 1;

    // printf("mesh[%zu]: \n", model->meshes.count);
    // for (size_t i = 0;
    //      i < model->meshes._[model->meshes.count - 1].vertices.count;
    //      i++)
    // {
    //     printf("vert[%zu]: pos(%f, %f, %f), norm(%f, %f, %f), tex(%f, "
    //            "%f)\n",
    //            i,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .position.x,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .position.y,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .position.z,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .normal.x,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .normal.y,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .normal.z,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .texture.x,
    //            model->meshes._[model->meshes.count - 1]
    //                .vertices._[i]
    //                .texture.y);
    // }
    // for (size_t i = 0;
    //      i < model->meshes._[model->meshes.count - 1].indices.count;
    //      i++)
    // {
    //     printf("ind[%zu]: %d\n", i,
    //            model->meshes._[model->meshes.count - 1].indices._[i]);
    // }

    // These pointers are now contained in the model structure, so there's
    // no need to free them.
    current_mesh.vertices._ = NULL;
    current_mesh.indices._ = NULL;
    current_mesh.materials._ = NULL;
    current_mesh.vertices.count = 0;
    current_mesh.indices.count = 0;
    current_mesh.materials.count = 0;
    current_mesh.VAO = 0;
}

void LetoProcessWavefrontMesh(leto_model_t *model,
                              char **material_file_path, const char *line)
{
    static leto_vec3_t *pos, *norm;
    static leto_vec2_t *tex;
    static size_t pos_count, norm_count, tex_count;

    switch (line[0])
    {
        case '#': break; // Comments. We do nothing.
        case 'o':
            // This keyword defines a new mesh object. We simply send the
            // old one to the model and reset the storage object.
            LetoUploadWavefrontMesh(model);
            mesh_filled = true;
            break;
        case 'm':
            (void)strncpy(*material_file_path, line + 7,
                          LETO_FILE_PATH_MAX);
            break;
        case 'v':
            if (line[1] == 'n')
            {
                norm =
                    realloc(norm, sizeof(leto_vec3_t) * (norm_count += 1));
                if (norm == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                char *string_left = malloc(64),
                     *string_left_original = string_left;
                if (string_left == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }
                strcpy(string_left, line + 3);

                norm[norm_count - 1].x = strtof(string_left, &string_left);
                norm[norm_count - 1].y = strtof(string_left, &string_left);
                norm[norm_count - 1].z = strtof(string_left, &string_left);
                free(string_left_original);
            }
            else if (line[1] == 't')
            {
                tex = realloc(tex, sizeof(leto_vec2_t) * (tex_count += 1));
                if (tex == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                char *string_left = malloc(64),
                     *string_left_original = string_left;
                if (string_left == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }
                strcpy(string_left, line + 3);

                tex[tex_count - 1].x = strtof(string_left, &string_left);
                tex[tex_count - 1].y = strtof(string_left, &string_left);
                free(string_left_original);
            }
            else
            {
                pos = realloc(pos, sizeof(leto_vec3_t) * (pos_count += 1));
                if (pos == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                char *string_left = malloc(64),
                     *string_left_original = string_left;
                if (string_left == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }
                strcpy(string_left, line + 2);

                pos[pos_count - 1].x = strtof(string_left, &string_left);
                pos[pos_count - 1].y = strtof(string_left, &string_left);
                pos[pos_count - 1].z = strtof(string_left, &string_left);
                free(string_left_original);
            }
            break;
        case 'f':
        {
            char *string_left = malloc(512),
                 *string_left_original = string_left;
            if (string_left == NULL)
            {
                fprintf(stderr, "Failed allocation.\n");
                return;
            }
            strcpy(string_left, line + 2);

            size_t vertices_in_face = 0;
            for (vertices_in_face = 0; vertices_in_face < 512;
                 vertices_in_face++)
            {
                char *segment_left = malloc(64),
                     *segment_left_original = segment_left;
                if (segment_left == NULL)
                {
                    fprintf(stderr, "Allocation failure.\n");
                    exit(EXIT_FAILURE);
                }

                long first_set_value =
                    strtol(string_left, &segment_left, 10);
                if (segment_left == string_left) break;

                current_mesh.vertices._ =
                    realloc(current_mesh.vertices._,
                            sizeof(leto_vertex_t) *
                                (current_mesh.vertices.count += 1));
                if (current_mesh.vertices._ == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .position = pos[first_set_value - 1];
                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .texture =
                    tex[strtol(segment_left + 1, &segment_left, 10) - 1];
                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .normal =
                    norm[strtol(segment_left + 1, &segment_left, 10) - 1];

                string_left = strdup(segment_left);
                free(segment_left_original);
            }

            //! this will fail for faces with > 4 vertices
            current_mesh.indices._ =
                realloc(current_mesh.indices._,
                        4 * (current_mesh.indices.count + 6));
            if (current_mesh.indices._ == NULL)
            {
                fprintf(stderr, "Failed allocation.\n");
                return;
            }

            current_mesh.indices._[current_mesh.indices.count] =
                current_mesh.vertices.count - 4;
            current_mesh.indices._[current_mesh.indices.count + 1] =
                current_mesh.vertices.count - 3;
            current_mesh.indices._[current_mesh.indices.count + 2] =
                current_mesh.vertices.count - 2;
            current_mesh.indices._[current_mesh.indices.count + 3] =
                current_mesh.vertices.count - 4;
            current_mesh.indices._[current_mesh.indices.count + 4] =
                current_mesh.vertices.count - 2;
            current_mesh.indices._[current_mesh.indices.count + 5] =
                current_mesh.vertices.count - 1;

            current_mesh.indices.count += 6;

            free(string_left_original);
            break;
        }
        case 's': // unimplemented
        default:  printf("Unknown WOBJ keyword. Line: %s\n", line);
    }
}

void LetoProcessWavefrontMaterial(const char *path)
{
    // we upload this to the mesh object, so no need to reference the model
}
