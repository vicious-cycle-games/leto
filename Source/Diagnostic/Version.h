/**
 * @file Version.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Defines some various version-related macros, like the release
 * cycle identifier and version numbers.
 * @date 2024-10-05
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the @file LICENSE.md file.
 */

#ifndef LETO__VERSION_H
#define LETO__VERSION_H

// Utility macros like stringify.
#include <Utilities/Macros.h>

/**
 * @brief A number representing the major version (X.x.x) of the currently
 * running Leto binaries. This will increment by one every time it's deemed
 * necessary to separate a version from its previous iterations. It will
 * never roll over.
 */
#define LETO_VERSION_MAJOR 0

/**
 * @brief A number representing the minor version (x.X.x) of the currently
 * running Leto binaries. This will increment by one every time an
 * interface is fundamentally changed or added. It rolls over every time a
 * new major version is released.
 */
#define LETO_VERSION_MINOR 1

/**
 * @brief A number representing the tweak version (x.x.X) of the currently
 * running Leto binaries. This will increment by one every time a change is
 * pushed, and rolls over every new major or minor version.
 */
#define LETO_VERSION_TWEAK 0

/**
 * @brief A string representing the release cycle name / ID of the
 * currently running Leto binaries. This can be one of four things, each of
 * which fairly self-explanatory. In order from least stable to stable;
 * "pre-alpha", "alpha", "beta", "release".
 */
#define LETO_VERSION_RCID "pre-alpha"

/**
 * @brief A string representing the full version of the currently running
 * Leto binaries.
 */
#define LETO_VERSION_STRING                                               \
    LETO_STRINGIFY(LETO_VERSION_MAJOR)                                    \
    "." LETO_STRINGIFY(LETO_VERSION_MINOR) "." LETO_STRINGIFY(            \
        LETO_VERSION_TWEAK) "-" LETO_VERSION_RCID

#endif // LETO__VERSION_H
