#include "wavefront.h"
#include <external/files.h>
#include <gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilities/memory/allocate.h>
#include <utilities/memory/free.h>

static leto_vec3_t *pos, *norm;
static leto_vec2_t *tex;
static size_t pos_count, norm_count, tex_count;

static bool VertexHandler_(const char *line)
{
    switch (line[1])
    {
        case 'n':
            norm = LetoReallocate(norm,
                                  sizeof(leto_vec3_t) * (norm_count += 1));
            if (norm == NULL) return false;
            StringToVec3(line, 3, &norm[norm_count - 1]);
            break;
        case 't':
            tex = LetoReallocate(tex,
                                 sizeof(leto_vec2_t) * (tex_count += 1));
            if (tex == NULL) return false;
            StringToVec2(line, 3, &tex[tex_count - 1]);
            break;
        case ' ':
            pos = LetoReallocate(pos,
                                 sizeof(leto_vec3_t) * (pos_count += 1));
            if (pos == NULL) return false;
            StringToVec3(line, 2, &pos[pos_count - 1]);
            break;
        default:
            fprintf(stderr, "Invalid vertex parameter.");
            return false;
    }
    return true;
}

static bool FaceHandler_(const char *line, leto_mesh_t *mesh)
{
    char *string_left = malloc(512), *string_left_original = string_left;
    if (string_left == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return false;
    }
    strcpy(string_left, line + 2);

    size_t vertices_in_face = 0;
    for (vertices_in_face = 0; vertices_in_face < 512; vertices_in_face++)
    {
        char *segment_left = malloc(64),
             *segment_left_original = segment_left;
        if (segment_left == NULL)
        {
            fprintf(stderr, "Allocation failure.\n");
            exit(EXIT_FAILURE);
        }

        long first_set_value = strtol(string_left, &segment_left, 10);
        if (segment_left == string_left) break;

        mesh->vertices._ =
            realloc(mesh->vertices._,
                    sizeof(leto_vertex_t) * (mesh->vertices.count += 1));
        if (mesh->vertices._ == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            return false;
        }

        mesh->vertices._[mesh->vertices.count - 1].position =
            pos[first_set_value - 1];
        mesh->vertices._[mesh->vertices.count - 1].texture =
            tex[strtol(segment_left + 1, &segment_left, 10) - 1];
        mesh->vertices._[mesh->vertices.count - 1].normal =
            norm[strtol(segment_left + 1, &segment_left, 10) - 1];

        string_left = strdup(segment_left);
        free(segment_left_original);
    }

    //! this will fail for faces with > 4 vertices
    mesh->indices._ =
        realloc(mesh->indices._, 4 * (mesh->indices.count + 6));
    if (mesh->indices._ == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return false;
    }

    mesh->indices._[mesh->indices.count] =
        (uint32_t)mesh->vertices.count - 4;
    mesh->indices._[mesh->indices.count + 1] =
        (uint32_t)mesh->vertices.count - 3;
    mesh->indices._[mesh->indices.count + 2] =
        (uint32_t)mesh->vertices.count - 2;
    mesh->indices._[mesh->indices.count + 3] =
        (uint32_t)mesh->vertices.count - 4;
    mesh->indices._[mesh->indices.count + 4] =
        (uint32_t)mesh->vertices.count - 2;
    mesh->indices._[mesh->indices.count + 5] =
        (uint32_t)mesh->vertices.count - 1;

    mesh->indices.count += 6;

    free(string_left_original);
    return true;
}

void LetoWavefrontProcessor(leto_model_t *model, const char *buffer)
{
    char *buffer_copy_ptr = (char *)buffer;
    char *line = strtok(buffer_copy_ptr, "\n");

    char *material_path = LetoAllocate(LETO_FILE_PATH_MAX);
    strcpy(material_path, "none");

    leto_mesh_t current_mesh = {{NULL, 0}, {NULL, 0}, {NULL, 0}, 0};

    do {
        switch (line[0])
        {
            case '#': break; // Comments. We do nothing.
            case 'o': LetoUploadWavefront(model, &current_mesh); break;
            case 'm':
                strncpy(material_path, line + 7, LETO_FILE_PATH_MAX);
                break;
            case 'v':
                if (!VertexHandler_(line))
                {
                    model->meshes._ = NULL;
                    return;
                }
                break;
            case 'f':
                if (!FaceHandler_(line, &current_mesh))
                {
                    model->meshes._ = NULL;
                    return;
                }
                break;
            case 's': // unimplemented
            default:  printf("Unknown WOBJ keyword. Line: %s\n", line);
        }
    } while (((line = strtok(NULL, "\n")) != NULL));

    LetoUploadWavefront(model, &current_mesh); // trailing mesh
    LetoFree((void **)&material_path);
}

void LetoMTLProcessor(const char *path)
{
    (void)path;
    // we upload this to the mesh object, so no need to reference the model
}

void LetoUploadWavefront(leto_model_t *model, leto_mesh_t *mesh)
{
    glGenVertexArrays(1, &mesh->VAO);

    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(mesh->VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is
    // sequential for all its items. The effect is that we can simply pass
    // a pointer to the struct and it translates perfectly to a glm::vec3/2
    // array which again translates to 3/2 floats which translates to a
    // byte array.
    glBufferData(GL_ARRAY_BUFFER,
                 mesh->vertices.count * sizeof(leto_vertex_t),
                 &mesh->vertices._[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh->indices.count * sizeof(uint32_t),
                 &mesh->indices._[0], GL_STATIC_DRAW);

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
    glBindVertexArray(0);

    model->meshes._ = realloc(
        model->meshes._, sizeof(leto_mesh_t) * (model->meshes.count + 1));
    if (model->meshes._ == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
    model->meshes._[model->meshes.count] = *mesh;
    model->meshes.count += 1;

    // These pointers are now contained in the model structure, so there's
    // no need to free them.
    mesh->vertices._ = NULL;
    mesh->indices._ = NULL;
    mesh->materials._ = NULL;
    mesh->vertices.count = 0;
    mesh->indices.count = 0;
    mesh->materials.count = 0;
    mesh->VAO = 0;
}
