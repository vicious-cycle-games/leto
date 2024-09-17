/**
 * @file strings.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for manipulating strings
 * in various ways.
 * @date 2024-09-17
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

#ifndef LETO__STRINGS__UTILITIES
#define LETO__STRINGS__UTILITIES

#include <stdbool.h>

bool FindSubstring(char *string, char *substring);
char *GetSubstring(const char *string, char *beginning, char *end);

#endif // LETO__STRINGS__UTILITIES
