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
    { 
        FT_PROFILE_FUNCTION; 
        FT_CORE_ASSERT(!s_Instance, "Application Already Exists!"); 
        s_Instance = this; 
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(std::bind(Application::OnEvent, this, std::placeholders::_1));
    }

    Application::~Application() { FT_PROFILE_FUNCTION; }

    void Application::Run()
    {
        FT_PROFILE_FUNCTION; 
        while (m_Running); // Game Initializer 
    }

    void Application::Close() { m_Running = false; }

    void Application::OnEvent(Event& e)
    {
        FT_PROFILE_FUNCTION;
        EventDispatcher dispatcher(e); 
        dispatcher.Dispatch<WindowCloseEvent>(
            std::bind(Application::OnWindowClose, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(
            std::bind(Application::OnWindowResize, this, std::placeholders::_1));
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled()) {
                break;
            }
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) 
    {
        FT_PROFILE_FUNCTION;
        m_Running = false; 
        return true; 
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        FT_PROFILE_FUNCTION; 
        // resize viewport (placeholder for now since I don't have a decent rendering system yet) 
        return false; 
    }


}