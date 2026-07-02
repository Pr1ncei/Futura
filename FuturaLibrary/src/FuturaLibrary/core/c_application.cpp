/*
    Prince, don't forget to use the event system so that it lessens the dependencies for it, thanks :3
    You don't want to duct tape the whole game without a system don't you?
*/
#include "pch.h"
#include "c_application.h"

#include "FuturaLibrary/events/e_EventJournal.h"
#include "FuturaLibrary/resources/r_ResourceManager.h"
#include "FuturaLibrary/utils/u_eventlog.h"
#include <GLFW/glfw3.h>

namespace FuturaLibrary
{
    Application* Application::s_Instance = nullptr; 
    Application::Application(const std::string& assetRoot)
        : Application(ApplicationConfig{ assetRoot })
    {
    }

    Application::Application(const ApplicationConfig& config)
        : m_AssetRoot(config.AssetRoot)
    { 
        FT_PROFILE_FUNCTION; 
        FT_CORE_ASSERT(!s_Instance, "Application Already Exists!"); 
        s_Instance = this; 

        EventJournal::Settings journalSettings;
        journalSettings.JournalPath = config.JournalPath;
        journalSettings.AllowEnvironmentOverride = config.AllowEnvironmentJournalOverride;
        switch (config.JournalMode)
        {
            case EventJournalMode::Record:
                journalSettings.JournalMode = EventJournal::Mode::Record;
                break;
            case EventJournalMode::Playback:
                journalSettings.JournalMode = EventJournal::Mode::Playback;
                break;
            case EventJournalMode::Off:
            default:
                journalSettings.JournalMode = EventJournal::Mode::Off;
                break;
        }

        m_EventJournal = std::make_unique<EventJournal>(journalSettings);
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        ResourceManager::Initialize(m_AssetRoot);
    }

    Application::~Application() { FT_PROFILE_FUNCTION; }

    void Application::Run()
    {
        FT_PROFILE_FUNCTION; 
        while (m_Running)
        {
            m_EventJournal->BeginFrame(m_FrameIndex);

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
                layer->OnRender();
            }

            m_EventJournal->DispatchPlaybackEvents([this](Event& event)
            {
                OnEvent(event);
            });

            m_Window->OnUpdate();
            m_FrameIndex++;
        }
    }

    void Application::Close() { m_Running = false; }

    Window& Application::GetWindow()
    {
        return *m_Window;
    }

    Application& Application::Get()
    {
        return *s_Instance;
    }

    void Application::OnEvent(Event& e)
    {
        FT_PROFILE_FUNCTION;
        if (m_EventJournal->ShouldIgnoreLiveEvent(e))
            return;

        m_EventJournal->RecordEvent(e);
        DispatchEvent(e);
    }

    void Application::DispatchEvent(Event& e)
    {
        EventDispatcher dispatcher(e); 
        dispatcher.Dispatch<WindowCloseEvent>(
            std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(
            std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
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
