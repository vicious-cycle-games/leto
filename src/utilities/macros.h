/**
 * @file macros.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide various utilities macros for the wider Leto
 * program. This includes things like stringify macros and conversion
 * macros.
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

#ifndef LETO__MACROS__UTILITES
#define LETO__MACROS__UTILITES

#define LETO_STR_INTERNAL_(x) #x
#define LETO_STRINGIFY(x) LETO_STR_INTERNAL_(x)

#endif // LETO__MACROS__UTILITES
