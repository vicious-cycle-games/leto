#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <utilities/memory/allocate.h>

#ifdef LETO_DEBUG

void LetoLog(const char *filename, const char *function, uint32_t line,
             const char *format, ...)
{
    printf("\033[32m%s() @ %s, ln. %d: ", function, filename, line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m\n");
}

void LetoLogWarning(const char *filename, const char *function,
                    uint32_t line, const char *format, ...)
{
    printf("\033[33m%s() @ %s, ln. %d: ", function, filename, line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m\n");
}

#endif
