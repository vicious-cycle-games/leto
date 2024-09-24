#ifndef LETO__WAVEFRONT__FORMAT
#define LETO__WAVEFRONT__FORMAT

#include "../structure.h"

void LetoWavefrontProcessor(leto_model_t *model, char **material_file_path,
                            const char *line);

void LetoMTLProcessor(const char *path);

void LetoWavefrontUploader(leto_model_t *model);

#endif // LETO__WAVEFRONT__FORMAT
