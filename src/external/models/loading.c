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

#include "formats/wavefront.h"         // Wavefront format
#include <external/files.h>            // Leto file utilities
#include <utilities/memory/allocate.h> // Leto memory allocators
#include <utilities/memory/free.h>     // Leto memory free-rs

#include <gl.h>     // OpenGL function macros
#include <stdio.h>  // Standard I/O functionality
#include <string.h> // Standard string manipulations

static void StringFormat_(leto_model_t *model, leto_model_format_t format,
                          const char *file_contents)
{
    // strtok advances the pointer by however many characters it takes
    // to hit "/n", so we make a copy as to not ruin the caller's pointer.
    char *file_contents_copy = (char *)file_contents,
         *line = strtok(file_contents_copy, "\n");

    model_processor_s processing_function = NULL;
    material_processor_s material_function = NULL;

    // For formats with material files separate, like Wavefront Object
    // files, we store the string file path in this value.
    char *material_file_path = LetoAllocateEmpty(LETO_FILE_PATH_MAX);
    strcpy(material_file_path, "none"); // strcpy cause 100% known length

    switch (format)
    {
        case wavefront:
            processing_function = LetoProcessWavefrontMesh;
            material_function = LetoProcessWavefrontMaterial;
            break;
        default:
            // Impossible.
            return;
    }

    // Loop until we hit the end of the file.
    while (line != NULL)
    {
        processing_function(model, &material_file_path, line);
        // NULL is passed because strtok saves the last value passed.
        line = strtok(NULL, "\n");
    }
    if (strcmp(material_file_path, "none") != 0)
        material_function(material_file_path);

    LetoUploadWavefrontMesh(model);
    LetoFree((void **)&material_file_path);
}

static void BinaryFormat_(leto_model_t *model, leto_model_format_t format,
                          const uint8_t *file_contents)
{
    (void)model;
    (void)format;
    (void)file_contents;
    // Nothing yet.
}

leto_model_t *LetoLoadModel(const char *path, leto_model_format_t format,
                            leto_vec3_t position, float rotation)
{
    if (path == NULL) return NULL;

    // We don't need the whole space (usually), so to prevent potential
    // gibberish path extensions, set all allocated bytes to 0.
    char *full_path = LetoAllocateEmpty(LETO_FILE_PATH_MAX);
    sprintf(full_path, ASSET_DIR "/models/%s", path);

    char *file_contents = NULL;
    LetoReadFile(&file_contents, full_path);
    if (file_contents == NULL)
    {
        fprintf(stderr, "Failed to read model files.\n");
        return NULL;
    }
    LetoFree((void **)&full_path); // The path value is no longer needed.

    leto_model_t *created_model = LetoAllocate(sizeof(leto_model_t));
    created_model->meshes.count = 0;
    created_model->meshes._ = NULL;
    created_model->position = position;
    created_model->rotation = rotation *
                              3.14159265358979323846264338327950288F /
                              100.0f; // mostly accurate radian conversion
    created_model->name = strdup(path);

    // Branch off to the different defined functions for the different
    // possible model formats.
    switch (format)
    {
        // The way we process the underlying array of bytes changes between
        // binary and string-based model formats, so split that here.
        case wavefront:
            StringFormat_(created_model, format, file_contents);
            break;
        case fbx:
            BinaryFormat_(created_model, format, (uint8_t *)file_contents);
            break;
        default:
            fprintf(stderr, "Unrecognized model type.\n");
            return NULL;
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
