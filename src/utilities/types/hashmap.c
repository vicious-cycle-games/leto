#include "hashmap.h"
#include <malloc.h>
#include <stdio.h>

leto_hashmap_t *LetoCreateHashmap(size_t starting_capacity)
{
    leto_hashmap_t *created_hashmap = malloc(sizeof(leto_hashmap_t));
    if (created_hashmap == NULL)
    {
        fprintf(stderr, "Failed allocation.\n");
        return NULL;
    }

    created_hashmap->pairs.count = 0;
    created_hashmap->pairs.space = starting_capacity;

    if (starting_capacity > 0)
    {
        created_hashmap->pairs._ =
            calloc(starting_capacity, sizeof(leto_pair_t));
        if (created_hashmap->pairs._ == NULL)
        {
            fprintf(stderr, "Failed allocation.\n");
            return NULL;
        }
    }

    return created_hashmap;
}

void LetoDestroyHashmap(leto_hashmap_t *map)
{
    if (map == NULL) return;

    free(map->pairs._);
    free(map);
}

uint64_t LetoHashmapGet(leto_hashmap_t *map, const char *key)
{
    (void)map;
    (void)key;
    return 0;
}

void LetoHashmapSet(leto_hashmap_t *map, const char *key, uint64_t value)
{
    (void)map;
    (void)key;
    (void)value;
}
