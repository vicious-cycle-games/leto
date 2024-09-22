#include "wavefront.h"
#include <external/files.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// A flag to tell us if the mesh has been filled at least once, to avoid
// the issue of the leading mesh declaration.
static bool mesh_filled = false;
static leto_mesh_t current_mesh;

void UploadWavefrontMesh(leto_model_t *model)
{
    if (!mesh_filled) return;

    model->meshes._ = realloc(
        model->meshes._, sizeof(leto_mesh_t) * (model->meshes.count + 1));
    if (model->meshes._ == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
    model->meshes._[model->meshes.count] = current_mesh;
    model->meshes.count += 1;

    printf("mesh[%zu]: \n", model->meshes.count);
    for (size_t i = 0;
         i < model->meshes._[model->meshes.count - 1].vertices.count; i++)
    {
        printf("vert[%zu]: pos(%f, %f, %f), norm(%f, %f, %f), tex(%f, "
               "%f)\n",
               i,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .position.x,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .position.y,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .position.z,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .normal.x,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .normal.y,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .normal.z,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .texture.x,
               model->meshes._[model->meshes.count - 1]
                   .vertices._[i]
                   .texture.y);
    }
    for (size_t i = 0;
         i < model->meshes._[model->meshes.count - 1].indices.count; i++)
    {
        printf("ind[%zu]: %d\n", i,
               model->meshes._[model->meshes.count - 1].indices._[i]);
    }

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

void ProcessWavefrontMesh(leto_model_t *model, char **material_file_path,
                          const char *line)
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
            UploadWavefrontMesh(model);
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

                norm[norm_count - 1].x = strtof(line + 3, NULL);
                norm[norm_count - 1].y = strtof(line + 11, NULL);
                norm[norm_count - 1].z = strtof(line + 18, NULL);
            }
            else if (line[1] == 't')
            {
                tex = realloc(tex, sizeof(leto_vec2_t) * (tex_count += 1));
                if (tex == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                tex[tex_count - 1].x = strtof(line + 2, NULL);
                tex[tex_count - 1].y = strtof(line + 11, NULL);
            }
            else
            {
                pos = realloc(pos, sizeof(leto_vec3_t) * (pos_count += 1));
                if (pos == NULL)
                {
                    fprintf(stderr, "Failed allocation.\n");
                    return;
                }

                pos[pos_count - 1].x = strtof(line + 2, NULL);
                pos[pos_count - 1].y = strtof(line + 11, NULL);
                pos[pos_count - 1].z = strtof(line + 20, NULL);
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

            for (size_t i = 1; i < 512; i++)
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
                current_mesh.indices._ =
                    realloc(current_mesh.indices._,
                            4 * (current_mesh.indices.count += 1));
                if (current_mesh.indices._ == NULL)
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

                //!! indices unimplemented
                current_mesh.indices._[current_mesh.indices.count - 1] =
                    current_mesh.vertices.count - 1;

                string_left = strdup(segment_left);
                free(segment_left_original);
            }

            free(string_left_original);
            break;
        }
        case 's': // unimplemented
        default:  printf("Unknown WOBJ keyword. Line: %s\n", line);
    }
}

void ProcessWavefrontMaterial(const char *path)
{
    // we upload this to the mesh object, so no need to reference the model
}
