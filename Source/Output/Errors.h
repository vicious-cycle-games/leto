#ifndef LETO__ERRORS_H
#define LETO__ERRORS_H

#include <Utilities/Types.h>

typedef enum leto_error
{
    no_error,
    failed_allocation,
    failed_glfw_init,
    failed_glad_init,
    failed_monitor_get,
    failed_window_create,
    no_display_func,
    error_count
} leto_error_t;

void LetoReportError(leto_boolean_t fatal, leto_error_t code,
                     leto_file_context_t context);

leto_error_t LetoGetError(void);

#endif // LETO__ERRORS_H
