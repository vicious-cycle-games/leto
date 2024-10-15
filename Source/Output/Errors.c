/**
 * @file Errors.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Implements the Leto error reporter interface.
 * @date 2024-10-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#include "Errors.h" // Public interface parent

#include <GLAD2/gl.h>   // OpenGL function pointers
#include <GLFW/glfw3.h> // GLFW functionality

#include <errno.h>  // The standard errno
#include <stdio.h>  // I/O functionality
#include <stdlib.h> // Instant exit functionality
#include <string.h> // Standard string utilities

/**
 * @brief Leto's version of @ref errno, the last error that was generated
 * by the process.
 */
static leto_error_code_t errid = no_error;

/**
 * @brief An array of all the possible error contexts. These are
 * self-explanatory.
 */
static leto_error_context_t error_contexts[error_count] = {
    {"no_error", "no error", leto},
    {"failed_allocation", "failed to allocate memory", leto},
    {"failed_file_open", "failed to open file", stdc},
    {"failed_file_position", "failed to position file", stdc},
    {"failed_file_tell", "failed to get file size", stdc},
    {"failed_file_read", "failed to read file", stdc},
    {"failed_glfw_init", "failed to init GLFW", glfw},
    {"failed_glad_init", "failed to init GLAD", glad},
    {"failed_monitor_get", "failed to get monitor", glfw},
    {"failed_window_create", "failed to create window", glfw},
    {"no_display_func", "no display function bound", leto},
    {"failed_shader", "failed to compile shader", glad},
    {"invalid_shader", "invalid shader value", glad}};

/**
 * OpenGLErrorString
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Get the error string of the current OpenGL error.
 *
 * @return const char* -- The error string we constructed.
 */
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

/**
 * GetAdditionalContext
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Get any additional context we need about the error, depending on
 * the domain of said error.
 *
 * @param domain The reported domain of the error.
 * @return const char* -- The additional context we got.
 */
static const char *GetAdditionalContext_(enum leto_error_domain domain)
{
    switch (domain)
    {
        case glfw:
        {
            const char *description;
            glfwGetError(&description);
            return description;
        }
        case glad: return OpenGLErrorString_();
        // Transform errno into a human-readable string description.
        case stdc: return strerror(errno);
        // There is no more information than we already have if Leto was
        // the perpetrator of the error.
        case leto: return "none";
        default:
            fprintf(stderr, "Failed to find valid error domain: \"%x\".\n",
                    domain);
            return NULL;
    }
}

void LetoReportError(bool fatal, leto_error_code_t code,
                     leto_file_context_t context)
{
    if (code == error_count)
    {
        fprintf(stderr,
                "Passed an invalid error code. Killing process.\n");
        abort();
    }

    leto_error_context_t error_context = error_contexts[code];
    fprintf(stderr,
            "\033[31mError @ %s() in %s, ln. %d:\n%s, \"%s\". Domain: %x "
            ":: %s\033[0m\n",
            context.function_name, context.file_name, context.line_number,
            error_context.full_name, error_context.description,
            error_context.domain,
            GetAdditionalContext_(error_context.domain));

    if (fatal == true) abort();
    errid = code;
}

leto_error_code_t LetoGetError(void) { return errid; }
