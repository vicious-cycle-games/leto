#ifndef LETO__TYPES__HASHMAP
#define LETO__TYPES__HASHMAP

// unfinished, will finish when needed

#include <stddef.h>
#include <stdint.h>

typedef struct leto_pair_t
{
    const char *key;
    uint64_t value;
    struct leto_pair_t *less;
    struct leto_pair_t *greater;
} leto_pair_t;

typedef struct leto_hashmap_t
{
    struct leto_pair_list_t
    {
        leto_pair_t *_;
        size_t count;
        size_t space;
    } pairs;
} leto_hashmap_t;

leto_hashmap_t *LetoCreateHashmap(size_t starting_capacity);
void LetoDestroyHashmap(leto_hashmap_t *map);

uint64_t LetoHashmapGet(leto_hashmap_t *map, const char *key);
void LetoHashmapSet(leto_hashmap_t *map, const char *key, uint64_t value);

#endif // LETO__TYPES__HASHMAP
