/**
 *  @file input.cpp
 * 
 *  @brief Implements the Input Class 
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 13, 2025 (12:17PM)
 *      Last Modified on:       December 13, 2025 (12:17PM)
 */


#include "input.h"
#include "camera.h"


void Input::Process(GLFWwindow* window)
{
    // ESC key basically exits out the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    // Movement System
    
}