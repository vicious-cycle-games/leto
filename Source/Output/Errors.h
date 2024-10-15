/**
 * @file Errors.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some basic error reporting tooling.
 * @date 2024-10-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__ERRORS_H
#define LETO__ERRORS_H

// The file context structure.
#include <Utilities/Types.h>

/**
 * @brief An enumerator describing all the possible error codes.
 */
typedef enum leto_error_code
{
    no_error,
    failed_allocation,
    failed_file_open,
    failed_file_position,
    failed_file_tell,
    failed_file_read,
    failed_glfw_init,
    failed_glad_init,
    failed_monitor_get,
    failed_window_create,
    no_display_func,
    failed_shader,
    invalid_shader,
    error_count
} leto_error_code_t;

/**
 * @brief The context structure for Leto's errors. Contains basic
 * information about the error, defined at compile time.
 */
typedef struct leto_error_context
{
    /**
     * @brief The full, string name of the error. This is the same value as
     * the @enum leto_error_code_t code passed into the @ref
     * LetoReportError function.
     */
    const char *full_name;
    /**
     * @brief A string description of the error.
     */
    const char *description;
    /**
     * @brief The various domains that an error can belong to. These will
     * decide where the reporter looks for extra information about the
     * error, like @ref errno or @ref glGetError.
     */
    enum leto_error_domain
    {
        /**
         * @brief The error originated from the GLFW library. Extra
         * information is polled via @ref glfwGetError.
         */
        glfw,
        /**
         * @brief The error originated from the GLAD2 library. Extra
         * information is polled via @ref glGetError.
         */
        glad,
        /**
         * @brief The error originated from the standard library. Extra
         * information is polled via @ref strerror/errno.
         */
        stdc,
        /**
         * @brief The error originated from Leto itself. Extra
         * information is not polled for.
         */
        leto
    } domain;
} leto_error_context_t;

/**
 * ReportError
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Report an error to the standard output and set @ref errid.
 *
 * @param fatal A boolean value representing if this error should be fatal
 * or not.
 * @param code The error code we need to report. If this value is @enum
 * error_count, we abort on an invalid error throw.
 * @param context The file context of the error; where it was triggered.
 * @return void -- Nothing.
 */
void LetoReportError(bool fatal, leto_error_code_t code,
                     leto_file_context_t context);

/**
 * GetError
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Get the current @ref errid.
 *
 * @return leto_error_code_t -- The current value of @ref errid.
 */
leto_error_code_t LetoGetError(void);

#endif // LETO__ERRORS_H
