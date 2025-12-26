#pragma once
#include "pch.h"
#include <memory>

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

#ifdef FT_DEBUG 
#define FT_ENABLE_ASSERTS
#endif 

#ifdef FT_ENABLE_ASSERTS 
#define FT_ASSERT(x, ...)										\
	{															\
		if (!(x))												\
		{														\
			FT_ERROR("Assertion Failed: {0}", __VA_ARGS__);		\
			__debugbreak();										\
		}														\
	}				
#define FT_CORE_ASSERT(x, ...)									\
	{															\
		if(!(x))												\
		{														\
			FT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			__debugbreak();										\
		}														\
	}

#else 
#define FT_ASSERT(x, ...)
#define FT_CORE_ASSERT(x, ...)
#endif 

#define BIT(x) (1 << x)

// I do not know why this isn't working 
#define FT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholder::_1)

namespace FuturaLibrary
{
	template <typename T> 
		using Scope = std::unique_ptr<T>;
	template <typename T> 
		using Ref = std::shared_ptr<T>; 
		typedef unsigned char texture_data;
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
#define GLM_ENABLE_EXPERIMENTAL