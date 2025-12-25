#pragma once

#include "c_core.h"
#include <memory>
#include "c_window.h"
#include "../events/e_Event.h"
#include "../events/e_AppEvent.h"

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

		inline Window& GetWindow() { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true; 
		static Application* s_Instance; 
	};
	// Define in Client -<-
	Application* CreateApplication(); 
}