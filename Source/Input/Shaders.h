#ifndef LETO__SHADERS_H
#define LETO__SHADERS_H

#include <Utilities/Types.h>

#define LETO_SHADER_PATH "./rss/shaders/"

unsigned int LetoLoadShader(const char *name);
void LetoUnloadShader(unsigned int id);

#endif // LETO__SHADERS_H
