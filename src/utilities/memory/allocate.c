#include "allocate.h"
#include "free.h"
#include <malloc.h>
#include <stdio.h>

void *LetoAllocate(size_t size)
{
    if (size == 0) return NULL;

    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }
    return ptr;
}

void *LetoAllocateEmpty(size_t size)
{
    if (size == 0) return NULL;

    void *ptr = calloc(1, size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }
    return ptr;
}

void *LetoReallocate(void *pointer, size_t new_size)
{
    if (new_size == 0)
    {
        LetoFree(pointer);
        return NULL;
    }

    void *ptr = realloc(pointer, new_size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }
    return ptr;
}
