#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "pch.h"
#include "c_camera.h"
#include <GLFW/glfw3.h>

class Input
{
public:
    void Process(GLFWwindow* m_window, Camera &camera, float deltaTime);
    void ProcessKeyboard(Camera &camera, Camera_Movement direction, GLfloat deltaTime);
    void ProcessMouseMovement(Camera &camera, GLfloat xoffset, GLfloat yoffset);
    void ProcessMouseScroll(Camera &camera, GLfloat yoffset);
    static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
    static void ScrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset);
};

#endif