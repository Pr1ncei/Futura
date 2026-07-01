/**
 *  @file c_window.cpp
 *
 *  @brief Implements the GLFW-backed Window used by the Futura application.
 *
 *  This file owns the platform window, OpenGL context creation, GLAD loading,
 *  swap/poll updates, and translation from GLFW callbacks into Futura events.
 *
 *      @author:             Prince Pamintuan
 *      @date:               December 08, 2025
 *      Last Modified on:    July 01, 2026
 */

#include "pch.h"
#include "c_window.h"

#include "FuturaLibrary/events/e_AppEvent.h"
#include "FuturaLibrary/events/e_KeyEvent.h"
#include "FuturaLibrary/events/e_MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace FuturaLibrary
{
	class GLFWWindow : public Window
	{
	public:
		explicit GLFWWindow(const WindowProps& props)
		{
			Initialize(props);
		}

		~GLFWWindow() override
		{
			Shutdown();
		}

		void OnUpdate() override
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }
		float GetAspectRatio() const override
		{
			return static_cast<float>(m_Data.Width) / static_cast<float>(m_Data.Height);
		}

		void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled) override
		{
			glfwSwapInterval(enabled ? 1 : 0);
			m_Data.VSync = enabled;
		}

		void SetMaximized(bool enabled) override
		{
			if (enabled)
				glfwMaximizeWindow(m_Window);
			else
				glfwRestoreWindow(m_Window);
		}

		void SetCursorVisibility() override
		{
			m_Data.CursorVisible = !m_Data.CursorVisible;
			glfwSetInputMode(
				m_Window,
				GLFW_CURSOR,
				m_Data.CursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
			);
		}

		bool IsVSync() const override { return m_Data.VSync; }
		double GetTime() const override { return glfwGetTime(); }
		void* GetNativeWindow() const override { return m_Window; }

	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			bool VSync = true;
			bool CursorVisible = true;
			EventCallbackFn EventCallback;
		};

		void Initialize(const WindowProps& props)
		{
			m_Data.Title = props.m_Title;
			m_Data.Width = props.m_Width;
			m_Data.Height = props.m_Height;

			if (s_GLFWWindowCount == 0)
			{
				int success = glfwInit();
				FT_CORE_ASSERT(success, "Could not initialize GLFW!");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef FT_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

			m_Window = glfwCreateWindow(
				static_cast<int>(props.m_Width),
				static_cast<int>(props.m_Height),
				m_Data.Title.c_str(),
				nullptr,
				nullptr
			);
			FT_CORE_ASSERT(m_Window, "Could not create GLFW window!");
			s_GLFWWindowCount++;

			glfwMakeContextCurrent(m_Window);
			glfwSetWindowUserPointer(m_Window, &m_Data);

			int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
			FT_CORE_ASSERT(status, "Could not initialize GLAD!");

			glViewport(0, 0, static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height));
			SetVSync(true);

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.Width = static_cast<unsigned int>(width);
				data.Height = static_cast<unsigned int>(height);
				glViewport(0, 0, width, height);

				WindowResizeEvent event(data.Width, data.Height);
				data.EventCallback(event);
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyTypedEvent event(static_cast<int>(keycode));
				data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				if (action == GLFW_PRESS)
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
				}
				else if (action == GLFW_RELEASE)
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
				data.EventCallback(event);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos), 0.0f, 0.0f);
				data.EventCallback(event);
			});
		}

		void Shutdown()
		{
			if (!m_Window)
				return;

			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
			s_GLFWWindowCount--;

			if (s_GLFWWindowCount == 0)
				glfwTerminate();
		}

	private:
		GLFWwindow* m_Window = nullptr;
		WindowData m_Data;

		static uint8_t s_GLFWWindowCount;
	};

	uint8_t GLFWWindow::s_GLFWWindowCount = 0;

	Window::~Window() = default;

	Window* Window::Create(const WindowProps& props)
	{
		return new GLFWWindow(props);
	}
}
