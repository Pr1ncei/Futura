#pragma once

#include "pch.h"
#include "FuturaLibrary/core/c_core.h"

namespace FuturaLibrary::FileIO
{
	std::string ExtractNameFromFilePath(const std::string& filePath)
	{
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = (lastSlash == std::string::npos ? 0 : lastSlash + 1); 
		auto lastDot = filePath.rfind('.');

		auto count = lastDot == std::string::npos
			? filePath.size() - lastSlash : lastDot - lastSlash; 

		return filePath.substr(lastSlash, count);
	}

	std::string ReadFile(const std::string& filePath)
	{
		FT_PROFILE_FUNCTION; 
		std::string result; 

		std::ifstream in(filePath, std::ios::in | std::ios::binary); 
		if (in)
		{
			in.seekg(0, std::ios::end); 
			result.resize(static_cast<size_t>(in.tellg()));
			in.seekg(0, std::ios::beg); 
			in.read(result.data(), result.size());
		}
		else
		{
			FT_CORE_ERROR("Could not open file {0}", filePath); 
		}

		return result; 

	}
}
