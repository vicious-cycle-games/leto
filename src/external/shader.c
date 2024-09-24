#include "shader.h"
#include "files.h"
#include <cam.h>
#include <gl.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

static void CheckShaderError_(unsigned int piece, unsigned int type)
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
}

unsigned int LetoLoadShader(const char *name)
{
    if (name == NULL) return 0;

    char *vraw = NULL, *fraw = NULL;
    LetoReadFileV(&vraw, ASSET_DIR "/shaders/%s.vert", name);
    LetoReadFileV(&fraw, ASSET_DIR "/shaders/%s.frag", name);
    const char *vcode = vraw, *fcode = fraw;

    unsigned int vid = glCreateShader(GL_VERTEX_SHADER),
                 fid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vid, 1, &vcode, NULL);
    glCompileShader(vid);
    CheckShaderError_(vid, GL_VERTEX_SHADER);

    glShaderSource(fid, 1, &fcode, NULL);
    glCompileShader(fid);
    CheckShaderError_(fid, GL_FRAGMENT_SHADER);

    free(vraw), free(fraw);

    unsigned int created_shader = glCreateProgram();
    glAttachShader(created_shader, vid);
    glAttachShader(created_shader, fid);
    glLinkProgram(created_shader);
    CheckShaderError_(created_shader, GL_PROGRAM);

    glDeleteShader(vid), glDeleteShader(fid);
    return created_shader;
}

void LetoUnloadShader(unsigned int id) { glDeleteProgram(id); }

void LetoSetProjection(unsigned int id, float fov, float ratio,
                       float znear, float zfar)
{
    glUseProgram(id);
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(fov), ratio, znear, zfar, projection);

    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE,
                       &projection[0][0]);
    glUseProgram(0);
}
