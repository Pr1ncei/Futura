#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/events/e_Event.h"

#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace FuturaLibrary
{
	class EventJournal
	{
	public:
		enum class Mode
		{
			Off = 0,
			Record,
			Playback
		};

		struct Settings
		{
			Mode JournalMode = Mode::Off;
			std::string JournalPath = "event_journal.ftj";
			bool AllowEnvironmentOverride = true;
		};

		explicit EventJournal(const Settings& settings = Settings());
		~EventJournal();

		void BeginFrame(uint64_t frameIndex);
		void RecordEvent(const Event& event);
		void DispatchPlaybackEvents(const std::function<void(Event&)>& dispatch);

		Mode GetMode() const { return m_Mode; }
		bool IsRecording() const { return m_Mode == Mode::Record; }
		bool IsPlayback() const { return m_Mode == Mode::Playback; }
		bool IsDispatchingPlayback() const { return m_DispatchingPlayback; }
		bool ShouldIgnoreLiveEvent(const Event& event) const;

		static const char* ModeToString(Mode mode);

	private:
		struct JournalEntry
		{
			uint64_t FrameIndex = 0;
			std::unique_ptr<Event> Event;
		};

		void InitializeFromSettings(const Settings& settings);
		void ApplyEnvironmentOverrides();
		bool SerializeEvent(const Event& event, std::string& outLine) const;
		std::unique_ptr<Event> DeserializeEvent(const std::string& line, uint64_t& outFrameIndex) const;
		void LoadPlaybackEntries();

	private:
		Mode m_Mode = Mode::Off;
		std::string m_Path = "event_journal.ftj";
		uint64_t m_CurrentFrameIndex = 0;
		bool m_DispatchingPlayback = false;

		std::ofstream m_RecordStream;
		std::vector<JournalEntry> m_PlaybackEntries;
		size_t m_PlaybackCursor = 0;
	};
}
