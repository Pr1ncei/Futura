#pragma once

#include "c_core.h"
#include <memory>

#include "c_window.h"
#include "FuturaLibrary/Core/c_LayerStack.h"
#include "FuturaLibrary/events/e_Event.h"
#include "FuturaLibrary/events/e_AppEvent.h"

namespace FuturaLibrary
{
	class FT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void Close(); 
		void OnEvent(Event& e);
		template <typename T> 
			requires(std::is_base_of_v<Layer, T>)
		void PushLayer()
		{
			m_LayerStack.PushLayer<T>(); 
		}
		template <typename T>
			requires(std::is_base_of_v<Layer, T>)
		void PushOverlay()
		{
			m_LayerStack.PushOverlay<T>();
		}
		template <typename T>
			requires(std::is_base_of_v<Layer, T>)
		T* GetLayer()
		{
			return m_LayerStack.GetLayer<T>();
		}
		inline Window& GetWindow() { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true; 
		static Application* s_Instance; 
		LayerStack m_LayerStack; 

		bool OnWindowClose(WindowCloseEvent& e); 
		bool OnWindowResize(WindowResizeEvent& e); 

	};
	// Define in Client -<-
	Application* CreateApplication(); 
}