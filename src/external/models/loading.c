#include "loading.h"
#include "formats/wavefront.h"
#include <external/files.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef void (*string_processing_function)(leto_model_t *, char **,
                                           const char *);
typedef void (*string_material_function)(const char *);

static void StringFormat_(leto_model_t *model, leto_model_format_t format,
                          const char *file_contents)
{
    // strtok advances the pointer by however many characters it takes
    // to hit "/n", so we make a copy as to not ruin the caller's pointer.
    char *file_contents_copy = (char *)file_contents,
         *line = strtok(file_contents_copy, "\n");

    string_processing_function processing_function = NULL;
    string_material_function material_function = NULL;

    // For formats with material files separate, like Wavefront Object
    // files, we store the string file path in this value.
    char *material_file_path = calloc(1, LETO_FILE_PATH_MAX);
    if (material_file_path == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
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
    free(material_file_path);
}

static void BinaryFormat_(leto_model_t *model, leto_model_format_t format,
                          const uint8_t *file_contents)
{
    (void)model;
    (void)format;
    (void)file_contents;
    // Nothing yet.
}

leto_model_t *LetoLoadModel(const char *path, leto_model_format_t format)
{
    if (path == NULL) return NULL;

    // We don't need the whole space (usually), so to prevent potential
    // gibberish path extensions, set all allocated bytes to 0.
    char *full_path = calloc(1, LETO_FILE_PATH_MAX);
    if (full_path == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }
    sprintf(full_path, ASSET_DIR "/models/%s", path);

    char *file_contents = NULL;
    LetoReadFile(&file_contents, full_path);
    free(full_path); // The path value is no longer needed.
    if (file_contents == NULL)
    {
        fprintf(stderr, "Failed to read model files.\n");
        return NULL;
    }

    leto_model_t *created_model = malloc(sizeof(leto_model_t));
    if (created_model == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }
    created_model->meshes.count = 0;
    created_model->meshes._ = NULL;

    // Branch off to the different defined functions for the different
    // possible model formats.
    switch (format)
    {
        // The way we process the underlying array of bytes changes between
        // binary and string-based model formats, so split that here.
        case wavefront:
            StringFormat_(created_model, format, file_contents);
            break;
        case stl:
            BinaryFormat_(created_model, format, (uint8_t *)file_contents);
            break;
        default:
            fprintf(stderr, "Unrecognized model type.\n");
            return NULL;
    }
    free(file_contents);

    return created_model;
}
