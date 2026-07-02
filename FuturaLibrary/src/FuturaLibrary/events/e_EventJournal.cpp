#include "pch.h"
#include "e_EventJournal.h"

#include "FuturaLibrary/events/e_AppEvent.h"
#include "FuturaLibrary/events/e_KeyEvent.h"
#include "FuturaLibrary/events/e_MouseEvent.h"
#include "FuturaLibrary/utils/u_eventlog.h"

#include <cstdlib>

namespace FuturaLibrary
{
	namespace
	{
		constexpr const char* JournalHeader = "FUTURA_EVENT_JOURNAL_V1";

		bool TryGetEnvironmentValue(const char* name, std::string& outValue)
		{
#ifdef _MSC_VER
			char* value = nullptr;
			size_t valueLength = 0;
			if (_dupenv_s(&value, &valueLength, name) != 0 || !value)
				return false;

			outValue = value;
			std::free(value);
			return true;
#else
			const char* value = std::getenv(name);
			if (!value)
				return false;

			outValue = value;
			return true;
#endif
		}

		bool IsInputOrWindowEvent(const Event& event)
		{
			switch (event.GetEventType())
			{
				case EventType::WindowClose:
				case EventType::WindowResize:
				case EventType::KeyPressed:
				case EventType::KeyReleased:
				case EventType::KeyTyped:
				case EventType::MouseButtonPressed:
				case EventType::MouseButtonReleased:
				case EventType::MouseMoved:
				case EventType::MouseScrolled:
					return true;
				default:
					return false;
			}
		}
	}

	EventJournal::EventJournal(const Settings& settings)
	{
		InitializeFromSettings(settings);

		if (m_Mode == Mode::Record)
		{
			m_RecordStream.open(m_Path, std::ios::out | std::ios::trunc);
			if (!m_RecordStream)
			{
				FT_CORE_ERROR("Event journal could not open {0} for recording", m_Path);
				m_Mode = Mode::Off;
				return;
			}

			m_RecordStream << JournalHeader << '\n';
			m_RecordStream.flush();
			FT_CORE_INFO("Event journal recording to {0}", m_Path);
		}
		else if (m_Mode == Mode::Playback)
		{
			LoadPlaybackEntries();
			FT_CORE_INFO("Event journal playback loaded {0} events from {1}", m_PlaybackEntries.size(), m_Path);
		}
	}

	EventJournal::~EventJournal()
	{
		if (m_RecordStream)
			m_RecordStream.flush();
	}

	void EventJournal::BeginFrame(uint64_t frameIndex)
	{
		m_CurrentFrameIndex = frameIndex;
	}

	void EventJournal::RecordEvent(const Event& event)
	{
		if (!IsRecording() || m_DispatchingPlayback || !IsInputOrWindowEvent(event))
			return;

		std::string line;
		if (!SerializeEvent(event, line))
			return;

		m_RecordStream << m_CurrentFrameIndex << ' ' << line << '\n';
		m_RecordStream.flush();
	}

	void EventJournal::DispatchPlaybackEvents(const std::function<void(Event&)>& dispatch)
	{
		if (!IsPlayback())
			return;

		m_DispatchingPlayback = true;

		while (m_PlaybackCursor < m_PlaybackEntries.size() &&
			m_PlaybackEntries[m_PlaybackCursor].FrameIndex == m_CurrentFrameIndex)
		{
			dispatch(*m_PlaybackEntries[m_PlaybackCursor].Event);
			m_PlaybackCursor++;
		}

		m_DispatchingPlayback = false;
	}

	bool EventJournal::ShouldIgnoreLiveEvent(const Event& event) const
	{
		if (!IsPlayback() || m_DispatchingPlayback)
			return false;

		return IsInputOrWindowEvent(event) && event.GetEventType() != EventType::WindowClose;
	}

	const char* EventJournal::ModeToString(Mode mode)
	{
		switch (mode)
		{
			case Mode::Record: return "record";
			case Mode::Playback: return "playback";
			case Mode::Off:
			default: return "off";
		}
	}

	void EventJournal::InitializeFromSettings(const Settings& settings)
	{
		m_Mode = settings.JournalMode;
		m_Path = settings.JournalPath;

		if (settings.AllowEnvironmentOverride)
			ApplyEnvironmentOverrides();
	}

	void EventJournal::ApplyEnvironmentOverrides()
	{
		std::string path;
		if (TryGetEnvironmentValue("FUTURA_EVENT_JOURNAL_PATH", path))
			m_Path = path;

		std::string modeValue;
		if (!TryGetEnvironmentValue("FUTURA_EVENT_JOURNAL_MODE", modeValue))
			return;

		if (modeValue == "record")
			m_Mode = Mode::Record;
		else if (modeValue == "playback")
			m_Mode = Mode::Playback;
		else
			m_Mode = Mode::Off;
	}

	bool EventJournal::SerializeEvent(const Event& event, std::string& outLine) const
	{
		std::ostringstream stream;

		switch (event.GetEventType())
		{
			case EventType::WindowClose:
				stream << "WindowClose";
				break;
			case EventType::WindowResize:
			{
				const auto& resizeEvent = static_cast<const WindowResizeEvent&>(event);
				stream << "WindowResize " << resizeEvent.GetWidth() << ' ' << resizeEvent.GetHeight();
				break;
			}
			case EventType::KeyPressed:
			{
				const auto& keyEvent = static_cast<const KeyPressedEvent&>(event);
				stream << "KeyPressed " << keyEvent.GetKeyCode() << ' ' << keyEvent.GetRepeatCount();
				break;
			}
			case EventType::KeyReleased:
			{
				const auto& keyEvent = static_cast<const KeyReleasedEvent&>(event);
				stream << "KeyReleased " << keyEvent.GetKeyCode();
				break;
			}
			case EventType::KeyTyped:
			{
				const auto& keyEvent = static_cast<const KeyTypedEvent&>(event);
				stream << "KeyTyped " << keyEvent.GetKeyCode();
				break;
			}
			case EventType::MouseButtonPressed:
			{
				const auto& mouseEvent = static_cast<const MouseButtonPressedEvent&>(event);
				stream << "MouseButtonPressed " << mouseEvent.GetMouseButton();
				break;
			}
			case EventType::MouseButtonReleased:
			{
				const auto& mouseEvent = static_cast<const MouseButtonReleasedEvent&>(event);
				stream << "MouseButtonReleased " << mouseEvent.GetMouseButton();
				break;
			}
			case EventType::MouseMoved:
			{
				const auto& mouseEvent = static_cast<const MouseMovedEvent&>(event);
				stream << "MouseMoved " << mouseEvent.GetX() << ' ' << mouseEvent.GetY() << ' '
					<< mouseEvent.GetXOffset() << ' ' << mouseEvent.GetYOffset();
				break;
			}
			case EventType::MouseScrolled:
			{
				const auto& mouseEvent = static_cast<const MouseScrolledEvent&>(event);
				stream << "MouseScrolled " << mouseEvent.GetXOffset() << ' ' << mouseEvent.GetYOffset();
				break;
			}
			default:
				return false;
		}

		outLine = stream.str();
		return true;
	}

	std::unique_ptr<Event> EventJournal::DeserializeEvent(const std::string& line, uint64_t& outFrameIndex) const
	{
		std::istringstream stream(line);
		std::string type;
		stream >> outFrameIndex >> type;

		if (type == "WindowClose")
			return std::make_unique<WindowCloseEvent>();

		if (type == "WindowResize")
		{
			unsigned int width = 0;
			unsigned int height = 0;
			stream >> width >> height;
			return std::make_unique<WindowResizeEvent>(width, height);
		}

		if (type == "KeyPressed")
		{
			int keyCode = 0;
			int repeatCount = 0;
			stream >> keyCode >> repeatCount;
			return std::make_unique<KeyPressedEvent>(keyCode, repeatCount);
		}

		if (type == "KeyReleased")
		{
			int keyCode = 0;
			stream >> keyCode;
			return std::make_unique<KeyReleasedEvent>(keyCode);
		}

		if (type == "KeyTyped")
		{
			int keyCode = 0;
			stream >> keyCode;
			return std::make_unique<KeyTypedEvent>(keyCode);
		}

		if (type == "MouseButtonPressed")
		{
			int button = 0;
			stream >> button;
			return std::make_unique<MouseButtonPressedEvent>(button);
		}

		if (type == "MouseButtonReleased")
		{
			int button = 0;
			stream >> button;
			return std::make_unique<MouseButtonReleasedEvent>(button);
		}

		if (type == "MouseMoved")
		{
			float x = 0.0f;
			float y = 0.0f;
			float xOffset = 0.0f;
			float yOffset = 0.0f;
			stream >> x >> y >> xOffset >> yOffset;
			return std::make_unique<MouseMovedEvent>(x, y, xOffset, yOffset);
		}

		if (type == "MouseScrolled")
		{
			float xOffset = 0.0f;
			float yOffset = 0.0f;
			stream >> xOffset >> yOffset;
			return std::make_unique<MouseScrolledEvent>(xOffset, yOffset);
		}

		return nullptr;
	}

	void EventJournal::LoadPlaybackEntries()
	{
		std::ifstream input(m_Path);
		if (!input)
		{
			FT_CORE_ERROR("Event journal could not open {0} for playback", m_Path);
			m_Mode = Mode::Off;
			return;
		}

		std::string line;
		if (!std::getline(input, line) || line != JournalHeader)
		{
			FT_CORE_ERROR("Event journal {0} has an invalid header", m_Path);
			m_Mode = Mode::Off;
			return;
		}

		while (std::getline(input, line))
		{
			uint64_t frameIndex = 0;
			std::unique_ptr<Event> event = DeserializeEvent(line, frameIndex);
			if (!event)
				continue;

			JournalEntry entry;
			entry.FrameIndex = frameIndex;
			entry.Event = std::move(event);
			m_PlaybackEntries.push_back(std::move(entry));
		}
	}
}
