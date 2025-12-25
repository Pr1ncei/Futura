/*
    Prince, don't forget to use the event system so that it lessens the dependencies for it, thanks :3
    You don't want to duct tape the whole game without a system don't you?
*/
#include "pch.h"
#include "c_application.h"

#include "FuturaLibrary/utils/u_eventlog.h"
#include <GLFW/glfw3.h>

namespace FuturaLibrary
{
    Application* Application::s_Instance = nullptr; 
    Application::Application()
    {;
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());






    }
}