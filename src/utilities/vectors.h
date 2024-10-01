#ifndef LETO__VECTORS__UTILITIES
#define LETO__VECTORS__UTILITIES

#include <stdbool.h>
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
    int32_t x, y, z;
} leto_ivec3_t;

typedef struct
{
    uint32_t x, y, z;
} leto_uvec3_t;

typedef struct
{
    uint8_t r, g, b;
} leto_rgb_t;

void LetoStringToVec3(const char *string, leto_vec3_t *vector);
void LetoStringToVec2(const char *string, leto_vec2_t *vector);

void LetoAddVec3(const leto_vec3_t *restrict a,
                 const leto_vec3_t *restrict b, leto_vec3_t *destination);
void LetoSubtractVec3(const leto_vec3_t *restrict a,
                      const leto_vec3_t *restrict b,
                      leto_vec3_t *destination);
void LetoCrossVec3(const leto_vec3_t *restrict a,
                   const leto_vec3_t *restrict b,
                   leto_vec3_t *destination);

bool LetoCheckVec3(const leto_vec3_t *restrict a,
                   const leto_vec3_t *restrict b);

#endif // LETO__VECTORS__UTILITIES
