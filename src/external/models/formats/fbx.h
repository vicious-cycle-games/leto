#ifndef LETO__FORMATS__FBX
#define LETO__FORMATS__FBX

#include "../structure.h"

void LetoFBXProcessor(leto_model_t *model, const uint8_t *file_contents);

void LetoFBXUploader(leto_model_t *model);

#endif // LETO__FORMATS__FBX
