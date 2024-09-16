/**
 * @file shader.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for loading and managing
 * OpenGL shaders using external GLSL documents.
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

#ifndef LETO__SHADER__EXTERNAL
#define LETO__SHADER__EXTERNAL

#define LETO_SHADER_PATH_MAX 256

unsigned int LetoLoadShader(const char *name);
void LetoUnloadShader(unsigned int id);

#endif // LETO__SHADER__EXTERNAL
