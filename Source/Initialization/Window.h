#ifndef LETO__WINDOW_H
#define LETO__WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow *LetoCreateWindow(const char *title);
void LetoDestroyWindow(GLFWwindow *window);

#endif // LETO__WINDOW_H
