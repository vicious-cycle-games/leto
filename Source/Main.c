#include <Initialization/Application.h>
#include <stdlib.h>

int main(void)
{
    leto_application_t *leto =
        LetoInitApplication(leto_false_t, leto_false_t);
    if (leto == NULL) exit(EXIT_FAILURE);

    LetoRunApplication(leto);

    LetoTerminateApplication(leto);
}
