#include <Initialization/Application.h>
#include <stdlib.h>

leto_boolean_t init(int width, int height)
{
    glViewport(0, 0, width, height);
    return leto_true_t;
}

void run(int width, int height)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(void)
{
    leto_application_t *leto =
        LetoInitApplication(leto_false_t, leto_false_t);
    if (leto == NULL) exit(EXIT_FAILURE);

    LetoBindDisplayInitFunc(leto, init);
    LetoBindDisplayFunc(leto, run);

    LetoRunApplication(leto);

    LetoTerminateApplication(leto);
}
