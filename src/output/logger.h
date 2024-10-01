#ifndef LETO__OUTPUT__LOGGER
#define LETO__OUTPUT__LOGGER

#include <stdint.h>

#define LETO_CONTEXT FILENAME, __func__, __LINE__

#ifdef LETO_DEBUG

void LetoLog(const char *filename, const char *function, uint32_t line,
             const char *format, ...);
void LetoLogWarning(const char *filename, const char *function,
                    uint32_t line, const char *format, ...);

#else
    #define LetoLog(...)
    #define LetoLogWarning(...)
#endif // LETO_DEBUG

#endif // LETO__OUTPUT__LOGGER
