/*
References: 
https://github.com/helpfulprojects/VoxelEngine/blob/main/VoxelEngine/src/VoxelEngine/Debug/Instrumentor.h#L73
https://gist.github.com/thiago-rezende/979bd63b3b8d3cd4eb1097367cf9b4bb
*/

#pragma once

#include "pch.h"

namespace FuturaLibrary
{
	struct ProfileResult
	{
		std::string Name; 
		long long Start, End; 
		uint32_t ThreadID; 
	};

	struct InstrumentationSession
	{
		std::string Name; 
	};

	// Creates a JSON File to Check the results of the program like the resources used 
	// Makes it easier to debug 
	class Instrumentor
	{
	private: 
		InstrumentationSession* m_CurrentSession; 
		std::ofstream m_OutputStream; 
		int m_ProfileCount; 

	public:
		Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}
		void BeginSession(const std::string& name, const std::string filePath = "debug.json") 
		{
			m_OutputStream.open(filePath);
			WriteHeader(); 
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void StartProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0) m_OutputStream << ","; 

			std::string name = result.Name; 
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{"; 
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void EndSession()
		{
			WriteFooter(); 
			m_OutputStream.close(); 
			delete m_CurrentSession; 
			m_CurrentSession = nullptr; 
			m_ProfileCount = 0; 
		}

		/* Helper Functions */
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush(); 
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}
	
		static Instrumentor& Get()
		{
			static Instrumentor instance; 
			return instance;
		}
	};

	class InstrumentorTimer
	{
		// To be implemented later, imma sleep (12-26-2025, 1:06am) 
	};
}
#ifdef FT_DEBUG 
#define FT_PROFILE_BEGIN_SESSION(name, filepath)
#define FT_PROFILE_END_SESSION 
#define FT_PROFILE_SCOPE(name) ZoneScopedN(name)
#define FT_PROFILE_FUNCTION ZoneScoped 
#else
#define FT_PROFILE_BEGIN_SESSION(name, filepath)
#define FT_PROFILE_END_SESSION 
#define FT_PROFILE_SCOPE(name)
#define FT_PROFILE_FUNCTION
#endif 
