/**
 *  @file r_ResourceManager.h
 *
 *  @brief Declares the Phase 2 resource manager for shader and texture assets.
 *
 *  ResourceManager centralizes asset-root resolution and caches common GPU
 *  resources so game layers do not repeatedly recreate shaders or textures.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 01, 2026
 *      Last Modified on:    July 01, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_texture.h"
#include "FuturaLibrary/resources/r_Model.h"

namespace FuturaLibrary
{
	class FT_API ResourceManager
	{
	public:
		static void Initialize(const std::string& assetRoot);

		static Ref<Shader> LoadShader(const std::string& name, const std::string& relativePath);
		static Ref<Shader> GetShader(const std::string& name);
		static bool HasShader(const std::string& name);

		static Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& relativePath);
		static Ref<Texture2D> GetTexture2D(const std::string& name);
		static bool HasTexture2D(const std::string& name);

		static Ref<Model> LoadModel(const std::string& name, const std::string& relativePath, const Ref<Shader>& shader);
		static Ref<Model> GetModel(const std::string& name);
		static bool HasModel(const std::string& name);

		static std::string ResolveAssetPath(const std::string& relativePath);
		static std::string NormalizeAssetPath(const std::string& path);

	private:
		static std::string s_AssetRoot;
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
		static std::unordered_map<std::string, Ref<Texture2D>> s_Textures;
		static std::unordered_map<std::string, Ref<Model>> s_Models;
		static std::unordered_map<std::string, std::string> s_ShaderPathAliases;
		static std::unordered_map<std::string, std::string> s_TexturePathAliases;
		static std::unordered_map<std::string, std::string> s_ModelPathAliases;
	};
}
