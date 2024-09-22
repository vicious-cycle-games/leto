#include "loading.h"
#include <external/files.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef void (*string_processing_function)(leto_model_t *, char **,
                                           const char *);
typedef void (*string_material_function)(const char *);

// A flag to tell us if the mesh has been filled at least once, to avoid
// the issue of the leading mesh declaration.
static bool mesh_filled = false;
static leto_mesh_t current_mesh;

static void UploadMesh(leto_model_t *model)
{
    if (!mesh_filled) return;

    model->meshes._[model->meshes.count - 1] = current_mesh;
    model->meshes.count += 1;

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

static void WaveFrontP_(leto_model_t *model, char **material_file_path,
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
            UploadMesh(model);
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
            current_mesh.vertices._ =
                realloc(current_mesh.vertices._,
                        sizeof(leto_vertex_t) *
                            (current_mesh.vertices.count += 1));
            if (current_mesh.vertices._ == NULL)
            {
                fprintf(stderr, "Failed allocation.\n");
                return;
            }

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

                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .position = pos[first_set_value];
                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .texture =
                    tex[strtol(segment_left + 1, &segment_left, 10)];
                current_mesh.vertices._[current_mesh.vertices.count - 1]
                    .normal =
                    norm[strtol(segment_left + 1, &segment_left, 10)];

                string_left = strdup(segment_left);
                free(segment_left_original);
            }

            free(string_left_original);
            break;
        case 's': // unimplemented
        default:  printf("Unknown WOBJ keyword. Line: %s\n", line);
    }

    // printf("%s\n", line);
}

static void WaveFrontM_(const char *path)
{
    // we upload this to the mesh object, no need to reference the model
}

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
            processing_function = WaveFrontP_;
            material_function = WaveFrontM_;
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
