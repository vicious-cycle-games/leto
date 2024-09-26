#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void StringToVec3(const char *string, size_t offset, leto_vec3_t *vector)
{
    char *string_left = malloc(1024), *string_left_original = string_left;
    if (string_left == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
    strcpy(string_left, string + offset);

    (*vector).x = strtof(string_left, &string_left);
    (*vector).y = strtof(string_left, &string_left);
    (*vector).z = strtof(string_left, &string_left);
    free(string_left_original);
}

void StringToVec2(const char *string, size_t offset, leto_vec2_t *vector)
{
    char *string_left = malloc(1024), *string_left_original = string_left;
    if (string_left == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return;
    }
    strcpy(string_left, string + offset);

    (*vector).x = strtof(string_left, &string_left);
    (*vector).y = strtof(string_left, &string_left);
    free(string_left_original);
}
