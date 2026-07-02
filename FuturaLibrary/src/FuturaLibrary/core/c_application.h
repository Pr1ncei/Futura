#pragma once

#include "c_core.h"
#include <memory>

#include "c_window.h"
#include "FuturaLibrary/Core/c_LayerStack.h"
#include "FuturaLibrary/events/e_Event.h"
#include "FuturaLibrary/events/e_AppEvent.h"

namespace FuturaLibrary
{
	class EventJournal;

	enum class EventJournalMode
	{
		Off = 0,
		Record,
		Playback
	};

	struct ApplicationConfig
	{
		std::string AssetRoot = "Futura/assets";
		EventJournalMode JournalMode = EventJournalMode::Off;
		std::string JournalPath = "event_journal.ftj";
		bool AllowEnvironmentJournalOverride = true;
	};

	class FT_API Application
	{
	public:
		explicit Application(const std::string& assetRoot = "Futura/assets");
		explicit Application(const ApplicationConfig& config);
		virtual ~Application();

		Application(const Application&) = delete; 
		Application& operator=(const Application&) = delete; 

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
		Window& GetWindow();
		static Application& Get();

	private:
		void DispatchEvent(Event& e);

	private:
		std::unique_ptr<Window> m_Window;
		std::string m_AssetRoot;
		bool m_Running = true; 
		static Application* s_Instance; 
		LayerStack m_LayerStack; 
		std::unique_ptr<EventJournal> m_EventJournal;
		uint64_t m_FrameIndex = 0;

		bool OnWindowClose(WindowCloseEvent& e); 
		bool OnWindowResize(WindowResizeEvent& e); 

	};
	// Define in Client -<-
	std::unique_ptr<Application> CreateApplication(const std::string& assetRoot = "Futura/assets");
}
