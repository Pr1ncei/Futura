/**
 *  @file r_ResourceManager.cpp
 *
 *  @brief Implements asset-root resolution and simple shader/texture caches.
 *
 *  This manager is intentionally small. It exists to remove duplicated resource
 *  setup from layers while Phase 2 stabilizes the renderer abstraction.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 01, 2026
 *      Last Modified on:    July 01, 2026
 */

#include "pch.h"
#include "r_ResourceManager.h"

#include <filesystem>

namespace FuturaLibrary
{
	std::string ResourceManager::s_AssetRoot = "Futura/assets";
	std::unordered_map<std::string, Ref<Shader>> ResourceManager::s_Shaders;
	std::unordered_map<std::string, Ref<Texture2D>> ResourceManager::s_Textures;

	void ResourceManager::Initialize(const std::string& assetRoot)
	{
		s_AssetRoot = assetRoot;
	}

	Ref<Shader> ResourceManager::LoadShader(const std::string& name, const std::string& relativePath)
	{
		if (HasShader(name))
			return GetShader(name);

		Ref<Shader> shader = Shader::Create(ResolveAssetPath(relativePath), "");
		s_Shaders[name] = shader;
		return shader;
	}

	Ref<Shader> ResourceManager::GetShader(const std::string& name)
	{
		FT_CORE_ASSERT(HasShader(name), "Shader does not exist in ResourceManager!");
		return s_Shaders[name];
	}

	bool ResourceManager::HasShader(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

	Ref<Texture2D> ResourceManager::LoadTexture2D(const std::string& name, const std::string& relativePath)
	{
		if (HasTexture2D(name))
			return GetTexture2D(name);

		Ref<Texture2D> texture = Texture2D::Create(ResolveAssetPath(relativePath));
		s_Textures[name] = texture;
		return texture;
	}

	Ref<Texture2D> ResourceManager::GetTexture2D(const std::string& name)
	{
		FT_CORE_ASSERT(HasTexture2D(name), "Texture does not exist in ResourceManager!");
		return s_Textures[name];
	}

	bool ResourceManager::HasTexture2D(const std::string& name)
	{
		return s_Textures.find(name) != s_Textures.end();
	}

	std::string ResourceManager::ResolveAssetPath(const std::string& relativePath)
	{
		auto appendPath = [](const std::string& root, const std::string& path)
		{
			if (path.empty())
				return root;

			char last = root.empty() ? '\0' : root.back();
			if (last == '/' || last == '\\')
				return root + path;

			return root + "/" + path;
		};

		std::string candidate = appendPath(s_AssetRoot, relativePath);
		if (std::filesystem::exists(candidate))
			return candidate;

		std::string projectDirCandidate = "../" + candidate;
		if (std::filesystem::exists(projectDirCandidate))
			return projectDirCandidate;

		std::string outputDirCandidate = "../../../" + candidate;
		if (std::filesystem::exists(outputDirCandidate))
			return outputDirCandidate;

		return candidate;
	}
}
