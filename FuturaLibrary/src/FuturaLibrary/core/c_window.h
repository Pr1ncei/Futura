/**
 *  @file window.h
 * 
 *  @brief Defines the Window class, which encapsulates GLFW window creation,
 *        context management, and basic event handling for OpenGL applications.
 * 
 *  This class handles initializing GLFW, creating a window, managing the OpenGL context,
 *  handling window resize events, and providing a simple interface for checking whether
 *  the window should close and updating the frame buffer.
 * 
 *      @author:                  Prince Pamintuan
 *      @date:                    December 08, 2025 (4:47PM)
 *      Last Modified on:         December 25, 2025 (11:59PM)
 */

#pragma once

#include "pch.h"
#include "c_core.h"
#include "../events/e_Event.h"

namespace FuturaLibrary
{
    struct WindowProps
    {
        std::string m_Title; 
        unsigned int m_Width, m_Height; 
        WindowProps(
            const std::string title = "Futura", 
            unsigned int width = 800,
            unsigned int height = 600
        ) : m_Title(title), m_Width(width), m_Height(height) { }
    };

    class FT_API Window
    {
    public: 
        using EventCallbackFn = std::function<void(Event&)>; 
        virtual ~Window(); 
        virtual void OnUpdate() = 0; 

        virtual unsigned int GetWidth() const = 0; 
        virtual unsigned int GetHeight() const = 0; 

        virtual float GetAspectRatio() const = 0; 

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0; 
        virtual void SetVSync(bool enabled) = 0; 
        virtual void SetMaximized(bool enabled) = 0; 
        virtual void SetCursorVisibility() = 0; 
        virtual void IsVsync() const = 0; 
        virtual double GetTime() const = 0; 

        virtual void* GetNativeWindow() const = 0; 

        static Window* Create(const WindowProps& props = WindowProps()); 

    };

}