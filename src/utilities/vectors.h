#ifndef LETO__VECTORS__UTILITIES
#define LETO__VECTORS__UTILITIES

#include <stddef.h>
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

typedef struct
{
    uint8_t r, g, b;
} leto_rgb_t;

void StringToVec3(const char *string, size_t offset, leto_vec3_t *vector);
void StringToVec2(const char *string, size_t offset, leto_vec2_t *vector);

#endif // LETO__VECTORS__UTILITIES
