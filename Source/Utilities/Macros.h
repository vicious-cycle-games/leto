#ifndef LETO__MACROS_H
#define LETO__MACROS_H

#include <stdlib.h>

#define LSTRIFY_INTERNAL_(x) #x
#define LETO_STRINGIFY(x) LSTRIFY_INTERNAL_(x)

#define LETO_ALLOC_OR_FAIL(variable, size)                                \
    variable = malloc(size);                                              \
    if (variable == NULL)                                                 \
    {                                                                     \
        LetoReportError(true, failed_allocation, LETO_FILE_CONTEXT);      \
    }

#endif // LETO__MACROS_H
