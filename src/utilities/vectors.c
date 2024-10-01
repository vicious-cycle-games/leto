#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LetoStringToVec3(const char *string, leto_vec3_t *vector)
{
    char *string_copy = strdup(string), *string_left = NULL;
    (*vector).x = strtof(string_copy, &string_left);
    (*vector).y = strtof(string_left, &string_left);
    (*vector).z = strtof(string_left, &string_left);
    free(string_copy);
}

void LetoStringToVec2(const char *string, leto_vec2_t *vector)
{
    char *string_copy = strdup(string), *string_left = NULL;
    (*vector).x = strtof(string_copy, &string_left);
    (*vector).y = strtof(string_left, &string_left);
    free(string_copy);
}

void LetoAddVec3(const leto_vec3_t *restrict a,
                 const leto_vec3_t *restrict b, leto_vec3_t *destination)
{
    destination->x = a->x + b->x;
    destination->y = a->y + b->y;
    destination->z = a->z + b->z;
}

void LetoSubtractVec3(const leto_vec3_t *restrict a,
                      const leto_vec3_t *restrict b,
                      leto_vec3_t *destination)
{
    destination->x = a->x - b->x;
    destination->y = a->y - b->y;
    destination->z = a->z - b->z;
}

void LetoCrossVec3(const leto_vec3_t *restrict a,
                   const leto_vec3_t *restrict b, leto_vec3_t *destination)
{
}

bool LetoCheckVec3(const leto_vec3_t *restrict a,
                   const leto_vec3_t *restrict b)
{
    return (a->x == b->x && a->y == b->y && a->z == b->z);
}
