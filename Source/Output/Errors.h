#ifndef LETO__ERRORS_H
#define LETO__ERRORS_H

#include <Utilities/Types.h>

typedef enum leto_error
{
    no_error,
    failed_allocation,
    failed_file_open,
    failed_file_position,
    failed_file_tell,
    failed_file_read,
    failed_glfw_init,
    failed_glad_init,
    failed_monitor_get,
    failed_window_create,
    no_display_func,
    failed_shader,
    invalid_shader,
    error_count
} leto_error_t;

void LetoReportError(bool fatal, leto_error_t code,
                     leto_file_context_t context);

leto_error_t LetoGetError(void);

#endif // LETO__ERRORS_H
