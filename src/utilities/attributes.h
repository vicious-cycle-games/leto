/**
 * @file attributes.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide platform-independent function attributes.
 * @date 2024-09-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#ifndef LETO__ATTRIBUTES__UTILITIES
#define LETO__ATTRIBUTES__UTILITIES

#include "platform.h"

#if defined(LETO_LINUX) || defined(LETO_APPLE)
    #define LETO_NORETURN _Noreturn void
#elif defined(LETO_WINDOWS)
    #define LETO_NORETURN __declspec(noreturn) void
#endif

#endif // LETO__ATTRIBUTES__UTILITIES
