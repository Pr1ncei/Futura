#pragma once
#include "pch.h"

#ifdef FT_PLATFORM_WINDOWS
	#ifdef FT_BUILD_DLL
		#define FT_API __declspec (dllexport)
	#else	
		#define FT_API __declspec (dllimport)
	#endif 

#elif defined(FT_PLATFORM_LINUX)
	#ifdef FT_BUILD_DLL
		#define	FT_API __attribute__((visibility("default")))
	#else
		#define FT_API
	#endif

#else
	#error Futura only supports Windows and Linux 

#endif 