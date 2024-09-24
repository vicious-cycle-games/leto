#ifndef LETO__MEMORY__ALLOCATE
#define LETO__MEMORY__ALLOCATE

#include <stddef.h>

void *LetoAllocate(size_t size);
void *LetoAllocateEmpty(size_t size);
void *LetoReallocate(void *pointer, size_t new_size);

#endif // LETO__MEMORY__ALLOCATE
