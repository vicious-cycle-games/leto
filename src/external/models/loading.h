/**
 * @file loading.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for the loading and
 * unloading of models, with all supported formats merged into a single
 * function.
 * @date 2024-09-24
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

#ifndef LETO__MODELS__LOADING
#define LETO__MODELS__LOADING

// Definitions of model and mesh structure objects.
#include "structure.h"

/**
 * @brief Load a model and all of its child meshes from the given file
 * path. The format supplied will decide how we try to interpret the data
 * within the file, and if given a false value the resulting mesh will
 * be malformed.
 * @returns A fully loaded model, or NULL if an error occurred. This value
 * was dynamically allocated, and @b must be freed by a subsequent call to
 * @ref LetoUnloadModel.
 *
 * @param path The file path of the model within the @dir rss/models
 * directory. If this value is NULL, this function will return NULL.
 * @param position A 3-dimensional array of floats that specifies the
 * global position of the mesh. This is in relation to (0,0) universal.
 * @param rotation The rotation, in degrees, that the model should be
 * globally rotated at. This value is converted to radians internally.
 */
leto_model_t *LetoLoadModel(const char *path, leto_vec3_t position,
                            float rotation);

/**
 * @brief Unload a previously loaded model, and all of its subcontexts.
 * @returns Nothing.
 *
 * @param model The model to unload. If this is NULL, the function returns
 * without doing anything.
 */
void LetoUnloadModel(leto_model_t *model);

#endif // LETO__MODELS__LOADING
