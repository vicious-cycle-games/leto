/**
 * @file Types.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some types that benefit the whole of Leto, like file
 * contexts and such.
 * @date 2024-10-14
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__TYPES_H
#define LETO__TYPES_H

// Standard boolean definitions.
#include <stdbool.h>
// Fixed-width integer types.
#include <stdint.h>

/**
 * @brief A structure to provide certain functions with an idea of where
 * they came from; file name, function name, etc.
 */
typedef struct leto_file_context
{
    /**
     * @brief The file name of the caller function.
     */
    const char *file_name;
    /**
     * @brief The name of the parent function.
     */
    const char *function_name;
    /**
     * @brief The line on which the error was reported.
     */
    uint32_t line_number;
} leto_file_context_t;

/**
 * @brief The default file context value.
 */
#define LETO_FILE_CONTEXT                                                 \
    (leto_file_context_t) { FILENAME, __func__, __LINE__ }

#endif // LETO__TYPES_H
