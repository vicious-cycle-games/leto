/**
 * @file Shaders.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Defines the public interface for Leto's shader loading process.
 * This includes setting normals and such that exist within shaders as
 * well.
 * @date 2024-10-07
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */

#ifndef LETO__SHADERS_H
#define LETO__SHADERS_H

// Standard boolean definitions.
#include <stdbool.h>

/**
 * @brief The directory in which all shaders should be placed, and that
 * which we assume them to be placed in the shader loading functions.
 */
#define LETO_SHADER_PATH "Shaders"

/**
 * LoadShader
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Load a full shader from its file within the provided directory.
 * This is not the name of an individual shader file, but rather the
 * subdirectory under the Leto shader directory that the shader's file(s)
 * reside in.
 *
 * @param name The name of the Leto shader directory subfolder that the
 * shader resides in.
 * @return unsigned int -- The OpenGL ID of the shader.
 */
unsigned int LetoLoadShader(const char *name);

/**
 * UnloadShader
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Unload the OpenGL information associated with the given shader
 * from memory.
 *
 * @param id The ID of the OpenGL shader we wish to unload.
 * @return void -- Nothing.
 */
void LetoUnloadShader(unsigned int id);

/**
 * SetProjectionMatrix
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Set the projection matrix stored in the given shader. If the
 * given shader is not valid, the function will return a failure.
 *
 * @param id The ID of the OpenGL shader whose projection we are to set.
 * @param fov The desired FOV of the view.
 * @param ratio The ratio of width to height that the matrix should conform
 * to. Note that if this value has already been passed once, and you do not
 * wish to change it, you do not have to provide a new value for it. Simply
 * pass in 0 and the same value as last time shall be passed.
 * @param znear The nearest value the matrix should tolerate in terms of
 * vertices.
 * @param zfar The farthest value the matrix should tolerate in terms of
 * vertices.
 * @return bool -- True for success, false if a failure occurred.
 */
bool LetoSetProjectionMatrix(unsigned int id, float fov, float ratio,
                             float znear, float zfar);

#endif // LETO__SHADERS_H
