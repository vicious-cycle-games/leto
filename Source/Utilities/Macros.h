/**
 * @file Macros.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some very basic utility macros for the wider Leto
 * project, like stringify and such.
 * @date 2024-10-14
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__MACROS_H
#define LETO__MACROS_H

// Standard memory allocation.
#include <stdlib.h>

/**
 * @brief The internal expansion macro for @ref LETO_STRINGIFY.
 */
#define LSTRIFY_INTERNAL_(x) #x

/**
 * @brief A small macro to stringify an input. Take the input `lorem
 * ipsum`, for example. Running it through this macro will return the
 * string "lorem ipsum".
 */
#define LETO_STRINGIFY(x) LSTRIFY_INTERNAL_(x)

/**
 * @brief The max path length Leto will tolerate.
 */
#define LETO_MAX_PATH_LENGTH 256

/**
 * @brief A macro to shorten the writing of a allocation checks. This
 * utilizes the @ref malloc function, and if @ref malloc fails, a fatal
 * @ref failed_allocation error is thrown.
 */
#define LETO_ALLOC_OR_FAIL(variable, size)                                \
    variable = malloc(size);                                              \
    if (variable == NULL)                                                 \
    {                                                                     \
        LetoReportError(true, failed_allocation, LETO_FILE_CONTEXT);      \
    }

#endif // LETO__MACROS_H
