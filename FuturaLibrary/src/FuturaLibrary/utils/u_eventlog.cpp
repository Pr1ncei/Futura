#include "pch.h"
#include "u_eventlog.h"

namespace FuturaLibrary
{
	std::shared_ptr<spdlog::logger> EventLog::s_CoreLogger;
	std::shared_ptr<spdlog::logger> EventLog::s_ClientLogger;

	void EventLog::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("FUTURA");
		s_CoreLogger->set_level(spdlog::level::trace); 
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace); 
	
	}
}