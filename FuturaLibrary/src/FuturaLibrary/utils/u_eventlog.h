#pragma once

#include "pch.h"
#include "../core/c_core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace FuturaLibrary
{
	class FT_API EventLog
	{
	public: 
		static void Init(); 

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger; 
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define FT_CORE_TRACE(...)		::FuturaLibrary::EventLog::GetCoreLogger()->trace(__VA_ARGS__)
#define FT_CORE_INFO(...)		::FuturaLibrary::EventLog::GetCoreLogger()->info(__VA_ARGS__)
#define FT_CORE_WARN(...)		::FuturaLibrary::EventLog::GetCoreLogger()->warn(__VA_ARGS__)
#define FT_CORE_ERROR(...)		::FuturaLibrary::EventLog::GetCoreLogger()->error(__VA_ARGS__)
#define FT_CORE_FATAL(...)		::FuturaLibrary::EventLog::GetCoreLogger()->fatal(__VA_ARGS__)

#define FT_TRACE(...)			::FuturaLibrary::EventLog::GetClientLogger()->trace(__VA_ARGS__)
#define FT_INFO(...)			::FuturaLibrary::EventLog::GetClientLogger()->info(__VA_ARGS__)
#define FT_WARN(...)			::FuturaLibrary::EventLog::GetClientLogger()->warn(__VA_ARGS__)
#define FT_ERROR(...)			::FuturaLibrary::EventLog::GetClientLogger()->error(__VA_ARGS__)
#define FT_FATAL(...)			::FuturaLibrary::EventLog::GetClientLogger()->fatal(__VA_ARGS__)