#ifndef LETO__WAVEFRONT__FORMAT
#define LETO__WAVEFRONT__FORMAT

#include "../structure.h"

void ProcessWavefrontMesh(leto_model_t *model, char **material_file_path,
                          const char *line);

void ProcessWavefrontMaterial(const char *path);

void UploadWavefrontMesh(leto_model_t *model);

#endif // LETO__WAVEFRONT__FORMAT
