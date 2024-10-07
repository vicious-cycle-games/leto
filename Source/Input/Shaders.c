/**
 * @file Shaders.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Implements Leto's shader loading interface, including some
 * various helper functions for setting important variables within the
 * shaders.
 * @implements Shaders.h
 * @date 2024-10-07
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Shaders.h"       // Public interface parent.
#include <Output/Errors.h> // Error reporting

#include <Utilities/Macros.h> // Utility macros

#include <CGLM/cam.h>  // GLM camera-related functions
#include <CGLM/mat4.h> // GLM matrix 4x4
#include <GLAD2/gl.h>  // OpenGL function pointers

#include <stdio.h>  // Standard I/O functions
#include <string.h> // String-related functionality

static bool CheckShaderError_(unsigned int piece, unsigned int type)
{
    int success_flag = false;
    char error_info[1024];

    if (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER)
    {
        glGetShaderiv(piece, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetShaderInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader compilation error:\n%s\n",
                    error_info);
            return false;
        }
    }
    else
    {
        glGetProgramiv(piece, GL_LINK_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetProgramInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader link error:\n%s\n", error_info);
            return false;
        }
    }

    return true;
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
        LetoReportError(false, failed_file_open, LETO_FILE_CONTEXT);
        return 0;
    }

    if (fseek(vertex_file, 0L, SEEK_END) == -1 ||
        fseek(fragment_file, 0L, SEEK_END) == -1)
    {
        LetoReportError(false, failed_file_position, LETO_FILE_CONTEXT);
        return 0;
    }

    long vertex_size = ftell(vertex_file);
    long fragment_size = ftell(fragment_file);
    if (vertex_size == -1 || fragment_size == -1)
    {
        LetoReportError(false, failed_file_tell, LETO_FILE_CONTEXT);
        return 0;
    }
    // Reset the file positioner.
    if (fseek(vertex_file, 0L, SEEK_SET) == -1 ||
        fseek(fragment_file, 0L, SEEK_SET) == -1)
    {
        LetoReportError(false, failed_file_position, LETO_FILE_CONTEXT);
        return 0;
    }

    char *vertex_raw = calloc((size_t)vertex_size + 1, 1);
    char *fragment_raw = calloc((size_t)fragment_size + 1, 1);
    if (vertex_raw == NULL || fragment_raw == NULL)
        LetoReportError(true, failed_allocation, LETO_FILE_CONTEXT);

    if (fread(vertex_raw, 1, (size_t)vertex_size, vertex_file) !=
            (size_t)vertex_size ||
        fread(fragment_raw, 1, (size_t)fragment_size, fragment_file) !=
            (size_t)fragment_size)
    {
        LetoReportError(false, failed_file_read, LETO_FILE_CONTEXT);
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
    if (CheckShaderError_(vertex, GL_VERTEX_SHADER) == false)
    {
        LetoReportError(false, failed_shader, LETO_FILE_CONTEXT);
        return 0;
    }

    glShaderSource(fragment, 1, &fragment_code, NULL);
    glCompileShader(fragment);
    if (CheckShaderError_(fragment, GL_FRAGMENT_SHADER) == false)
    {
        LetoReportError(false, failed_shader, LETO_FILE_CONTEXT);
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

bool LetoSetProjectionMatrix(unsigned int id, float fov, float ratio,
                             float znear, float zfar)
{
    // To make it easier when calling, cache the width/height ratio.
    static float ratio_storage;
    if (ratio != 0) ratio_storage = ratio;

    // Check to make sure the shader is valid.
    glUseProgram(id);
    if (glGetError() != GL_NO_ERROR)
    {
        LetoReportError(false, invalid_shader, LETO_FILE_CONTEXT);
        return false;
    }

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(fov), ratio_storage, znear, zfar, projection);
    // This assumes the variable is named "projection_matrix" in the
    // shader code.
    glUniformMatrix4fv(glGetUniformLocation(id, "projection_matrix"), 1,
                       GL_FALSE, &projection[0][0]);

    return true;
}
