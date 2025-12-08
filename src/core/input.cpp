#include "input.h"

void Input::Process(GLFWwindow* window)
{
    // ESC key basically exits out the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}