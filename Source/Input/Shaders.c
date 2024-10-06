#include "Shaders.h"
#include <CGLM/cglm.h>
#include <GLAD2/gl.h>
#include <Output/Errors.h>
#include <Utilities/Macros.h>
#include <stdio.h>
#include <string.h>

static leto_boolean_t CheckShaderError_(unsigned int piece,
                                        unsigned int type)
{
    int success_flag = leto_false_t;
    char error_info[1024];

    if (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER)
    {
        glGetShaderiv(piece, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetShaderInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader compilation error:\n%s\n",
                    error_info);
            return leto_false_t;
        }
    }
    else
    {
        glGetProgramiv(piece, GL_LINK_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetProgramInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader link error:\n%s\n", error_info);
            return leto_false_t;
        }
    }

    return leto_true_t;
}

unsigned int LetoLoadShader(const char *name)
{
    if (name == NULL) return 0;

    size_t path_length = 29 + strlen(name);
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

    if (fseek(vertex_file, 0L, SEEK_END) == -1 ||
        fseek(fragment_file, 0L, SEEK_END) == -1)
    {
        LetoReportError(leto_false_t, failed_file_position,
                        LETO_FILE_CONTEXT);
        return 0;
    }

    long vertex_size = ftell(vertex_file);
    long fragment_size = ftell(fragment_file);
    if (vertex_size == -1 || fragment_size == -1)
    {
        LetoReportError(leto_false_t, failed_file_tell, LETO_FILE_CONTEXT);
        return 0;
    }
    // Reset the file positioner.
    if (fseek(vertex_file, 0L, SEEK_SET) == -1 ||
        fseek(fragment_file, 0L, SEEK_SET) == -1)
    {
        LetoReportError(leto_false_t, failed_file_position,
                        LETO_FILE_CONTEXT);
        return 0;
    }

    char *vertex_raw = calloc((size_t)vertex_size + 1, 1);
    char *fragment_raw = calloc((size_t)fragment_size + 1, 1);
    if (vertex_raw == NULL || fragment_raw == NULL)
        LetoReportError(leto_true_t, failed_allocation, LETO_FILE_CONTEXT);

    if (fread(vertex_raw, 1, (size_t)vertex_size, vertex_file) !=
            (size_t)vertex_size ||
        fread(fragment_raw, 1, (size_t)fragment_size, fragment_file) !=
            (size_t)fragment_size)
    {
        LetoReportError(leto_false_t, failed_file_read, LETO_FILE_CONTEXT);
        return 0;
    }

    vertex_raw[vertex_size] = '\0';
    fragment_raw[fragment_size] = '\0';

    fclose(vertex_file);
    fclose(fragment_file);

    const char *vertex_code = vertex_raw, *fragment_code = fragment_raw;
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER),
                 fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex, 1, &vertex_code, NULL);
    glCompileShader(vertex);
    if (CheckShaderError_(vertex, GL_VERTEX_SHADER) == leto_false_t)
    {
        LetoReportError(leto_false_t, failed_shader, LETO_FILE_CONTEXT);
        return 0;
    }

    glShaderSource(fragment, 1, &fragment_code, NULL);
    glCompileShader(fragment);
    if (CheckShaderError_(fragment, GL_FRAGMENT_SHADER) == leto_false_t)
    {
        LetoReportError(leto_false_t, failed_shader, LETO_FILE_CONTEXT);
        return 0;
    }

    free(vertex_raw), free(fragment_raw);

    unsigned int created_shader = glCreateProgram();
    glAttachShader(created_shader, vertex);
    glAttachShader(created_shader, fragment);
    glLinkProgram(created_shader);
    CheckShaderError_(created_shader, GL_PROGRAM);

    glDeleteShader(vertex), glDeleteShader(fragment);
    return created_shader;
}

void LetoUnloadShader(unsigned int id) { glDeleteProgram(id); }

void LetoSetProjection(unsigned int id, float fov, float ratio,
                       float znear, float zfar)
{
    glUseProgram(id);
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(fov), ratio, znear, zfar, projection);

    glUniformMatrix4fv(glGetUniformLocation(id, "projection_matrix"), 1,
                       GL_FALSE, &projection[0][0]);
}
