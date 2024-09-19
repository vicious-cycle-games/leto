#include "meshes.h"
#include "files.h"
#include <stdio.h>
#include <string.h>
#include <utilities/strings.h>

static const char *GetMeshFormatExt_(leto_mesh_format_t format)
{
    switch (format)
    {
        case wavefront: return "obj";
        default:        return "unk";
    }
}

// separate file?
static void SynthesizeVector3_(const char *line, vec3 *vector)
{
    char *string_left = malloc(19), *str_left_original = string_left;
    if (string_left == NULL)
    {
        fprintf(stderr, "Allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    (*vector)[0] = strtof(line, &string_left);
    (*vector)[1] = strtof(line, &string_left);
    (*vector)[2] = strtof(line, &string_left);
    free(str_left_original);
}

static void ParseFace_(const char *line, leto_face_t *face)
{
    char *string_left = malloc(512), *str_left_original = string_left;
    if (string_left == NULL)
    {
        fprintf(stderr, "Allocation failure.\n");
        exit(EXIT_FAILURE);
    }
    string_left = (char *)line;

    // calloc so if the num of elements < 3, 0 placeholder
    face->elements = calloc(3, sizeof(ivec3));
    if (face->elements == NULL)
    {
        fprintf(stderr, "Allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    for (face->element_count = 1;
         face->element_count < LETO_MAX_FACE_ELEMENTS;
         face->element_count++)
    {
        char *string_left_first = malloc(64),
             *string_left_first_original = string_left_first;
        if (string_left == NULL)
        {
            fprintf(stderr, "Allocation failure.\n");
            exit(EXIT_FAILURE);
        }

        printf("%zu\n", face->element_count);
        int first_set_value =
            (int)strtol(string_left, &string_left_first, 10);
        if (string_left_first == string_left) break;

        if (face->element_count > 3)
        {
            face->elements = realloc(face->elements,
                                     sizeof(ivec3) * face->element_count);
            if (face->elements == NULL)
            {
                fprintf(stderr, "Allocation failure.\n");
                exit(EXIT_FAILURE);
            }
        }

        face->elements[face->element_count - 1][0] = first_set_value;
        face->elements[face->element_count - 1][1] =
            (int)strtol(string_left_first + 1, &string_left_first, 10);
        face->elements[face->element_count - 1][2] =
            (int)strtol(string_left_first + 1, &string_left_first, 10);

        string_left = strdup(string_left_first);
        free(string_left_first_original);
    }
    free(str_left_original);
}

static void InterpretLine_(leto_mesh_t *mesh, char *line,
                           char **material_file_name,
                           char ***material_list,
                           size_t *material_list_length)
{
    if (FindCharacter(line, '#')) return;

    if (FindCharacter(line, 'o')) mesh->name = strdup(line + 2);
    else if (FindSubstring(line, "mtllib"))
        *material_file_name = (line + 7);
    else if (FindSubstring(line, "usemtl"))
    {
        *material_list = realloc(
            *material_list, sizeof(char *) * (*material_list_length += 1));
        if (*material_list == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            exit(EXIT_FAILURE);
        }

        *material_list[*material_list_length - 1] = strdup(line + 7);
    }
    else if (FindSubstring(line, "v "))
    {
        mesh->vertices = realloc(mesh->vertices,
                                 sizeof(vec3) * (mesh->vertex_count += 1));
        if (mesh->vertices == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            exit(EXIT_FAILURE);
        }

        SynthesizeVector3_(line + 2,
                           &mesh->vertices[mesh->vertex_count - 1]);
    }
    else if (FindSubstring(line, "vn"))
    {
        mesh->normals = realloc(mesh->normals,
                                sizeof(vec3) * (mesh->normal_count += 1));
        if (mesh->normals == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            exit(EXIT_FAILURE);
        }
        SynthesizeVector3_(line + 3,
                           &mesh->normals[mesh->normal_count - 1]);
    }
    else if (FindSubstring(line, "vt"))
    {
        mesh->texture_coords =
            realloc(mesh->texture_coords,
                    sizeof(vec3) * (mesh->texture_coord_count += 1));
        if (mesh->texture_coords == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            exit(EXIT_FAILURE);
        }
        SynthesizeVector3_(
            line + 3,
            &mesh->texture_coords[mesh->texture_coord_count - 1]);
        // printf("%f :: %f :: %f\n",
        //        mesh->texture_coords[mesh->texture_coord_count - 1][0],
        //        mesh->texture_coords[mesh->texture_coord_count - 1][1],
        //        mesh->texture_coords[mesh->texture_coord_count - 1][2]);
    }
    else if (FindSubstring(line, "f "))
    {
        mesh->faces = realloc(mesh->faces, sizeof(leto_face_t) *
                                               (mesh->face_count += 1));
        if (mesh->faces == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            exit(EXIT_FAILURE);
        }
        ParseFace_(line + 2, &mesh->faces[mesh->face_count - 1]);
    }
}

leto_mesh_t *LetoLoadMesh(const char *name, leto_mesh_format_t format)
{
    leto_mesh_t *created_mesh = calloc(1, sizeof(leto_mesh_t));
    if (created_mesh == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }

    char **mesh_contents = NULL;
    size_t mesh_line_count = 0;
    LetoReadLinesV(&mesh_contents, &mesh_line_count,
                   ASSET_DIR "/meshes/%s.%s", name,
                   GetMeshFormatExt_(format));
    if (mesh_contents == NULL) return NULL;

    char *material_file_name = NULL, **material_names = NULL;
    size_t material_names_length = 0;
    for (size_t i = 0; i < mesh_line_count; i++)
    {
        char *current_line = mesh_contents[i];
        InterpretLine_(created_mesh, current_line, &material_file_name,
                       &material_names, &material_names_length);
        printf("res[%zu] = %s\n", i, current_line);
    }

    for (size_t i = 0; i < mesh_line_count; i++) free(mesh_contents[i]);
    free(mesh_contents);
    return created_mesh;
}

void LetoUnloadMesh(leto_mesh_t *mesh)
{
    (void)mesh;
    return;
}
