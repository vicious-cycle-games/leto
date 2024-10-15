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

#include "Shaders.h" // Public interface parent
#include "Files.h"   // File operations

#include <Output/Errors.h>    // Error reporting
#include <Utilities/Macros.h> // Utility macros

#include <CGLM/cam.h>  // GLM camera-related functions
#include <CGLM/mat4.h> // GLM matrix 4x4
#include <GLAD2/gl.h>  // OpenGL function pointers

#include <stdio.h>  // Standard I/O functions
#include <string.h> // String-related functionality

/**
 * CheckShaderError
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief See if any errors have occurred during the compilation of
 * shaders. The error reporting method for this is different than regular
 * @ref glGetError, so we print a string in advance that explains the
 * error.
 *
 * @param piece The item we're trying to get an error report from.
 * @param type The type of item; @ref GL_VERTEX_SHADER, @ref
 * GL_FRAGMENT_SHADER, or @ref GL_PROGRAM.
 * @return bool -- True for no error, false for an error occurred.
 */
static bool CheckShaderError_(unsigned int piece, unsigned int type)
{
    int success_flag = false; // OpenGL reports into an integer.
    char error_info[1024];

    if (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER)
    {
        glGetShaderiv(piece, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetShaderInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader compilation error:\n%s\n",
                    error_info);
        }
    }
    else
    {
        glGetProgramiv(piece, GL_LINK_STATUS, &success_flag);
        if (!success_flag)
        {
            glGetProgramInfoLog(piece, 1024, NULL, error_info);
            fprintf(stderr, "OpenGL shader link error:\n%s\n", error_info);
        }
    }
    return success_flag;
}

/**
 * CompileShader
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Compile the given type of shader within the given shader folder.
 *
 * @param shader A storage position for the OpenGL ID of the shader object.
 * @param name The name of the shader's containing folder.
 * @param type The OpenGL type of the shader, i.e @ref GL_VERTEX_SHADER,
 * @ref GL_FRAGMENT_SHADER, etc.
 * @return void -- Nothing.
 */
static void CompileShader_(unsigned int *shader, const char *name,
                           unsigned int type)
{
    char *buffer = NULL;
    LetoReadFile(&buffer, 0, LETO_SHADER_PATH "/%s/%s", name,
                 (type == GL_VERTEX_SHADER ? "vert.vs" : "frag.fs"));
    if (buffer == NULL) *shader = 0;

    const char *code = buffer;
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &code, NULL);
    glCompileShader(*shader);

    if (CheckShaderError_(*shader, type) == false)
    {
        LetoReportError(false, failed_shader, LETO_FILE_CONTEXT);
        *shader = 0;
        return;
    }

    free(buffer);
}

unsigned int LetoLoadShader(const char *name)
{
    if (name == 0) return 0;

    unsigned int vertex, fragment;
    CompileShader_(&vertex, name, GL_VERTEX_SHADER);
    CompileShader_(&fragment, name, GL_FRAGMENT_SHADER);
    if (vertex == 0 || fragment == 0) return 0;

    unsigned int created_shader = glCreateProgram();
    glAttachShader(created_shader, vertex);
    glAttachShader(created_shader, fragment);
    glLinkProgram(created_shader);

    if (CheckShaderError_(created_shader, GL_PROGRAM) == false)
    {
        LetoReportError(false, failed_shader, LETO_FILE_CONTEXT);
        return 0;
    }

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
