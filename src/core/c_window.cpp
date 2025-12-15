/*
    This manages the window by initializing, updating, and terminating

    Author: Prince Pamintuan
    Date: December 08, 2025 (4:47PM)

*/

#include "c_window.h"

bool Window::Initialize(Camera* camera, Input* input)
{   
    if (!glfwInit())
    {
        std::cout << "ERROR::Failed to initialize GLFW\n";
        glfwTerminate();
        return false; 
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_window = glfwCreateWindow(800, 600, "Futura", NULL, NULL);
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, Input::MouseCallback);
    glfwSetScrollCallback(m_window, Input::ScrollCallback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD\n";
        glfwTerminate();
        return false;
    }

    m_context.camera = camera; 
    m_context.input = input; 

    glfwSetWindowUserPointer(m_window, &m_context);

    glClearColor(0.25f, 0.15f, 0.75f, 1.0f);
    return true;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::Update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::Terminate()
{
    glfwTerminate();
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}