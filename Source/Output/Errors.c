#include "Errors.h"
#include <GLAD2/gl.h>
#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static leto_error_t errid = no_error;

typedef struct leto_error_context
{
    const char *full_name;
    const char *description;
    uint8_t domain;
} leto_error_context_t;

#define LETO_GLFW_DOMAIN 0xD1
#define LETO_GLAD_DOMAIN 0xD2
#define LETO_STDC_DOMAIN 0xD3
#define LETO_LETO_DOMAIN 0xD4

static leto_error_context_t error_contexts[error_count] = {
    {"no_error", "no error", LETO_LETO_DOMAIN},
    {"failed_allocation", "failed to allocate memory", LETO_LETO_DOMAIN},
    {"failed_file_open", "failed to open file", LETO_STDC_DOMAIN},
    {"failed_file_position", "failed to position file", LETO_STDC_DOMAIN},
    {"failed_file_tell", "failed to get file size", LETO_STDC_DOMAIN},
    {"failed_file_read", "failed to read file", LETO_STDC_DOMAIN},
    {"failed_glfw_init", "failed to init GLFW", LETO_GLFW_DOMAIN},
    {"failed_glad_init", "failed to init GLAD", LETO_GLAD_DOMAIN},
    {"failed_monitor_get", "failed to get monitor", LETO_GLFW_DOMAIN},
    {"failed_window_create", "failed to create window", LETO_GLFW_DOMAIN},
    {"no_display_func", "no display function bound", LETO_LETO_DOMAIN},
    {"failed_shader", "failed to compile shader", LETO_GLAD_DOMAIN},
    {"invalid_shader", "invalid shader value", LETO_GLAD_DOMAIN}};

const char *OpenGLErrorString_(void)
{
    unsigned int error = glGetError();
    switch (error)
    {
        case GL_NO_ERROR:          return "no_error";
        case GL_INVALID_ENUM:      return "invalid_enum";
        case GL_INVALID_VALUE:     return "invalid_value";
        case GL_INVALID_OPERATION: return "invalid_operation";
        case GL_STACK_OVERFLOW:    return "stack_overflow";
        case GL_STACK_UNDERFLOW:   return "stack_underflow";
        case GL_OUT_OF_MEMORY:     return "out_of_memory";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "invalid_framebuffer";
        default:
            fprintf(stderr, "Unrecognized OpenGL error: %d.\n", error);
            return NULL;
    }
}

const char *GLFWErrorString_(void)
{
    const char *description;
    glfwGetError(&description);
    return description;
}

static const char *GetAdditionalContext_(uint8_t domain)
{
    switch (domain)
    {
        case LETO_GLFW_DOMAIN: return GLFWErrorString_();
        case LETO_GLAD_DOMAIN: return OpenGLErrorString_();
        case LETO_STDC_DOMAIN: return strerror(errno);
        case LETO_LETO_DOMAIN: return "none";
        default:
            fprintf(stderr, "Failed to find valid error domain: \"%x\".\n",
                    domain);
            return NULL;
    }
}

void LetoReportError(bool fatal, leto_error_t code,
                     leto_file_context_t context)
{
    if (code == error_count)
    {
        fprintf(stderr,
                "Passed an invalid error code. Killing process.\n");
        exit(EXIT_FAILURE);
    }

    leto_error_context_t error_context = error_contexts[code];
    fprintf(stderr,
            "\033[31mError @ %s() in %s, ln. %d:\n%s, \"%s\". Domain: %x "
            ":: %s\033[0m\n",
            context.function_name, context.file_name, context.line_number,
            error_context.full_name, error_context.description,
            error_context.domain,
            GetAdditionalContext_(error_context.domain));

    if (fatal == true) exit(EXIT_FAILURE);
    errid = code;
}

leto_error_t LetoGetError(void) { return errid; }
