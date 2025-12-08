#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private: 
    GLFWwindow* m_window;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    GLFWwindow* getWindow() const { return m_window; }
    
    bool Initialize();
    bool ShouldClose();
    void Update();  
    void Terminate();
};

#endif