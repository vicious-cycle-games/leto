#ifndef LETO__TYPES_H
#define LETO__TYPES_H

#include <stdint.h>

typedef uint8_t leto_boolean_t;
#define leto_true_t 1
#define leto_false_t 0

typedef struct leto_file_context
{
    const char *file_name;
    const char *function_name;
    uint32_t line_number;
} leto_file_context_t;

#define LETO_FILE_CONTEXT                                                 \
    (leto_file_context_t) { FILENAME, __func__, __LINE__ }

#endif // LETO__TYPES_H
