#include "free.h"
#include <malloc.h>
#include <stddef.h>

void LetoFree(void **pointer)
{
    if (pointer == NULL || *pointer == NULL) return;
    free(*pointer);
    *pointer = NULL;
}
