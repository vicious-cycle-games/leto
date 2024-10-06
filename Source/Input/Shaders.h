#ifndef LETO__SHADERS_H
#define LETO__SHADERS_H

#include <Utilities/Types.h>

#define LETO_SHADER_PATH "./Resources/Shaders/"

unsigned int LetoLoadShader(const char *name);
void LetoUnloadShader(unsigned int id);
void LetoSetProjection(unsigned int id, float fov, float ratio,
                       float znear, float zfar);

#endif // LETO__SHADERS_H
