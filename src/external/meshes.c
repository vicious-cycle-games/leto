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

static void InterpretLine_(leto_mesh_t *mesh, char *line,
                           char **material_name)
{
    if (FindSubstring(line, "# ")) return;
    // else if (FindSubstring(line, "mtllib")) *material_name = line;
    // else if (FindSubstring(line, "o ")) mesh->name =
}

leto_mesh_t *LetoLoadMesh(const char *name, leto_mesh_format_t format)
{
    leto_mesh_t *created_mesh = malloc(sizeof(leto_mesh_t));
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

    char *material_name = NULL;
    for (int i = 0; i < mesh_line_count; i++)
    {
        char *current_line = mesh_contents[i];
        InterpretLine_(created_mesh, current_line, &material_name);
        printf("res[%d] = %s\n", i, current_line);
    }

    for (int i = 0; i < mesh_line_count; i++) free(mesh_contents[i]);
    free(mesh_contents);
    return created_mesh;
}
