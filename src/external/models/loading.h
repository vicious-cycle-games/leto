#ifndef LETO__MODELS__LOADING
#define LETO__MODELS__LOADING

#include "structure.h"

/**
 * @brief Load a model and all of its child meshes from the given file
 * path. The format supplied will decide how we try to interpret the data
 * within the file, and if given a malformed value the resulting mesh will
 * be malformed.
 * @returns A fully loaded model, or NULL if an error occurred. This value
 * was dynamically allocated, and @b must be freed by a subsequent call to
 * @ref LetoUnloadModel.
 *
 * @param path The file path of the model within the @dir rss/models
 * directory. If this value is NULL, this function will return NULL.
 * @param format The format in which we should interpret the model file's
 * data.
 */
leto_model_t *LetoLoadModel(const char *path, leto_model_format_t format,
                            leto_vec3_t position, float rotation);

#endif // LETO__MODELS__LOADING
