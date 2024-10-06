#include "Shaders.h"
#include <GLAD2/gl.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>
#include <stdio.h>
#include <string.h>

unsigned int LetoLoadShader(const char *name)
{
    size_t path_length = 23 + strlen(name);
    char *vertex_path, *fragment_path;

    LETO_ALLOC_OR_FAIL(vertex_path, path_length);
    LETO_ALLOC_OR_FAIL(fragment_path, path_length);
    snprintf(vertex_path, path_length, LETO_SHADER_PATH "%s/vert.vs",
             name);
    snprintf(fragment_path, path_length, LETO_SHADER_PATH "%s/frag.fs",
             name);

    FILE *vertex_file = fopen(vertex_path, "rb");
    FILE *fragment_file = fopen(fragment_path, "rb");
    free(vertex_path);
    free(fragment_path);

    if (vertex_file == NULL || fragment_file == NULL)
    {
        LetoReportError(leto_false_t, failed_file_open, LETO_FILE_CONTEXT);
        return 0;
    }

    fclose(vertex_file);
    fclose(fragment_file);

    unsigned int program = 0;
    return program;
}

void LetoUnloadShader(unsigned int id) { glDeleteProgram(id); }
