#ifndef LETO__WAVEFRONT__FORMAT
#define LETO__WAVEFRONT__FORMAT

#include "../structure.h"

void LetoProcessWavefrontMesh(leto_model_t *model,
                              char **material_file_path, const char *line);

void LetoProcessWavefrontMaterial(const char *path);

void LetoUploadWavefrontMesh(leto_model_t *model);

#endif // LETO__WAVEFRONT__FORMAT
