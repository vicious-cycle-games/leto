#ifndef LETO__TYPES_H
#define LETO__TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef struct leto_file_context
{
    const char *file_name;
    const char *function_name;
    uint32_t line_number;
} leto_file_context_t;

#define LETO_FILE_CONTEXT                                                 \
    (leto_file_context_t) { FILENAME, __func__, __LINE__ }

#endif // LETO__TYPES_H
