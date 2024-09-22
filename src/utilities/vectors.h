#ifndef LETO__VECTORS__UTILITIES
#define LETO__VECTORS__UTILITIES

#include <stdint.h>

typedef struct
{
    float x, y, z;
} leto_vec3_t;

typedef struct
{
    float x, y, z, w;
} leto_vec4_t;

typedef struct
{
    float x, y;
} leto_vec2_t;

typedef struct
{
    uint32_t x, y, z;
} leto_ivec3_t;

#endif // LETO__VECTORS__UTILITIES
