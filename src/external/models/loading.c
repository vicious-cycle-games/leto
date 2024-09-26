/**
 * @file loading.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the implementation of the public model loading
 * interface defined in @file loading.h.
 * @date 2024-09-24
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#include "loading.h" // Public interface parent

#include "formats/fbx.h"       // FBX format
#include "formats/wavefront.h" // Wavefront format

#include <external/files.h>            // Leto file utilities
#include <utilities/macros.h>          // Degrees to radians
#include <utilities/memory/allocate.h> // Leto memory allocators
#include <utilities/memory/free.h>     // Leto memory free-rs

#include <gl.h>     // OpenGL function macros
#include <stdio.h>  // Standard I/O functionality
#include <string.h> // Standard string manipulations

leto_model_t *LetoLoadModel(const char *path, leto_model_format_t format,
                            leto_vec3_t position, float rotation)
{
    if (path == NULL) return NULL;

    char *full_path = LetoAllocate(LETO_FILE_PATH_MAX);
    sprintf(full_path, ASSET_DIR "/models/%s", path);

    char *file_contents = NULL;
    LetoReadFile(&file_contents, full_path);
    if (file_contents == NULL) return NULL;
    LetoFree((void **)&full_path); // The path value is no longer needed.

    leto_model_t *created_model = LetoAllocateEmpty(sizeof(leto_model_t));
    created_model->name = strdup(path);
    created_model->position = position;
    created_model->rotation = DEGREE_TO_RAD(rotation);

    // Branch off to the different defined functions for the different
    // possible model formats.
    switch (format)
    {
        // The way we process the underlying array of bytes changes between
        // binary and string-based model formats, so split that here.
        case wavefront:
        {
            // char *material_path = LetoAllocate(LETO_FILE_PATH_MAX);
            // strcpy(material_path, "none");

            // char *line = strtok(file_contents, "\n");
            // do LetoWavefrontProcessor(created_model, &material_path,
            // line); while (((line = strtok(NULL, "\n")) != NULL));

            // if (strcmp(material_path, "none") != 0)
            //     LetoMTLProcessor(material_path);
            // LetoUploadWavefront(created_model); // trailing mesh
            // LetoFree((void **)&material_path);

            LetoWavefrontProcessor(created_model, file_contents);
            if (created_model->meshes._ == NULL)
            {
                LetoFree((void **)&created_model);
                return NULL;
            }
            break;
        }
        case fbx:
            // Unimplemented.
            LetoFBXProcessor(created_model, (uint8_t *)file_contents);
            break;
        default: fprintf(stderr, "Unrecognized model type.\n"); break;
    }
    LetoFree((void **)&file_contents);
    return created_model;
}

void LetoUnloadModel(leto_model_t *model)
{
    if (model == NULL) return;

    LetoFree((void **)&model->name);
    model->position.x = 0;
    model->position.y = 0;
    model->position.z = 0;
    model->rotation = 0.0f;

    for (size_t i = 0; i < model->meshes.count; i++)
    {
        leto_mesh_t *current_mesh = &model->meshes._[i];

        // Delete the underlying OpenGL object, not just our ID of it.
        glDeleteVertexArrays(1, &current_mesh->VAO);
        current_mesh->VAO = 0;

        LetoFree((void **)&current_mesh->vertices._);
        LetoFree((void **)&current_mesh->indices._);
        current_mesh->vertices.count = 0;
        current_mesh->indices.count = 0;

        for (size_t j = 0; j < current_mesh->materials.count; j++)
            LetoFree((void **)&current_mesh->materials._[j].name);
        LetoFree((void **)&current_mesh->materials._);
        current_mesh->materials.count = 0;
    }
}
