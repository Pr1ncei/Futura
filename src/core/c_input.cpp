/**
 *  @file input.cpp
 * 
 *  @brief Implements the Input Class 
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 13, 2025 (12:17PM)
 *      Last Modified on:       December 15, 2025 (8:07PM)
 */

#include "c_input.h"
#include "c_window_context.h"

// Main Input Process
void Input::Process(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
  
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(camera, FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(camera, BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(camera, LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(camera, RIGHT, deltaTime);

}
 
void Input::ProcessKeyboard(Camera &camera, Camera_Movement direction, float deltaTime)
{
    float velocity = camera.MovementSpeed * deltaTime; 
    if (direction == FORWARD)
        camera.Position += camera.Front * velocity;  
    if (direction == BACKWARD)
        camera.Position -= camera.Front * velocity; 
    if (direction == LEFT)
        camera.Position -= camera.Right * velocity; 
    if (direction == RIGHT)
        camera.Position += camera.Right * velocity;
    camera.Position.y = 0.0f;   // This is good to set a true fps camera
}

void Input::ProcessMouseMovement(Camera &camera, GLfloat xoffset, GLfloat yoffset)
{
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void Input::ProcessMouseScroll(Camera &camera, GLfloat yoffset)
{
    camera.Zoom -= (GLfloat)yoffset;
    if (camera.Zoom < 1.0f)
        camera.Zoom = 1.0f; 
    if (camera.Zoom > 45.0f)
        camera.Zoom = 45.0f;
}

void Input::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    auto* context = static_cast<WindowContext *>(
        glfwGetWindowUserPointer(window)
    );

    if (!context || !context->input || !context->camera) return;

    static bool firstMouse = true; 
    static float lastX; 
    static float lastY; 


    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (firstMouse)
    {
        lastX = xpos; 
        lastY = ypos; 
        firstMouse = false; 
        return; 
    }

    float xoffset = xpos - lastX; 
    float yoffset = lastY - ypos;
    lastX = xpos; 
    lastY = ypos; 

    context->input->ProcessMouseMovement(
        *context->camera,
        xoffset,
        yoffset
    );

}

void Input::ScrollCallback(GLFWwindow* window, double /*xoffdset*/, double yoffset)
{
    auto* context = static_cast<WindowContext *>(
        glfwGetWindowUserPointer(window)
    );

    if (!context || !context->input || !context->camera) return;

    Input* input = static_cast<Input *>(glfwGetWindowUserPointer(window));
    if (input)
        input->ProcessMouseScroll(*context->camera, static_cast<float>(yoffset));
}