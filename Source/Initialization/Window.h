#ifndef LETO__WINDOW_H
#define LETO__WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow *CreateWindow(const char *title);
void DestroyWindow(GLFWwindow *window);

#endif // LETO__WINDOW_H
