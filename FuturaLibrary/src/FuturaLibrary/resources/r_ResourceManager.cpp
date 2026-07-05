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

#ifdef FT_ENABLE_ASSIMP
	#include <assimp/Importer.hpp>
	#include <assimp/material.h>
	#include <assimp/postprocess.h>
	#include <assimp/scene.h>
#endif

namespace FuturaLibrary
{
	std::string ResourceManager::s_AssetRoot = "Futura/assets";
	std::unordered_map<std::string, Ref<Shader>> ResourceManager::s_Shaders;
	std::unordered_map<std::string, Ref<Texture2D>> ResourceManager::s_Textures;
	std::unordered_map<std::string, Ref<Model>> ResourceManager::s_Models;
	std::unordered_map<std::string, std::string> ResourceManager::s_ShaderPathAliases;
	std::unordered_map<std::string, std::string> ResourceManager::s_TexturePathAliases;
	std::unordered_map<std::string, std::string> ResourceManager::s_ModelPathAliases;

	namespace
	{
		std::filesystem::path NormalizePathObject(const std::filesystem::path& path)
		{
			std::error_code error;
			std::filesystem::path absolutePath = std::filesystem::absolute(path, error);
			if (error)
				absolutePath = path;

			std::filesystem::path normalized = std::filesystem::weakly_canonical(absolutePath, error);
			if (error)
				normalized = absolutePath.lexically_normal();

			return normalized;
		}

		std::string NormalizePathString(const std::filesystem::path& path)
		{
			return NormalizePathObject(path).generic_string();
		}

#ifdef FT_ENABLE_ASSIMP
		glm::vec3 ToVec3(const aiVector3D& value)
		{
			return { value.x, value.y, value.z };
		}

		glm::vec2 ToVec2(const aiVector3D& value)
		{
			return { value.x, value.y };
		}

		MeshData ConvertAssimpMesh(const aiMesh& assimpMesh)
		{
			MeshData meshData;
			meshData.Vertices.reserve(assimpMesh.mNumVertices);

			for (uint32_t i = 0; i < assimpMesh.mNumVertices; i++)
			{
				Vertex vertex;
				vertex.Position = ToVec3(assimpMesh.mVertices[i]);

				if (assimpMesh.HasTextureCoords(0))
					vertex.TexCoord = ToVec2(assimpMesh.mTextureCoords[0][i]);
				if (assimpMesh.HasNormals())
					vertex.Normal = ToVec3(assimpMesh.mNormals[i]);
				if (assimpMesh.HasTextureCoords(1))
					vertex.LightmapTexCoord = ToVec2(assimpMesh.mTextureCoords[1][i]);
				else
					vertex.LightmapTexCoord = vertex.TexCoord;

				meshData.Vertices.push_back(vertex);
			}

			for (uint32_t i = 0; i < assimpMesh.mNumFaces; i++)
			{
				const aiFace& face = assimpMesh.mFaces[i];
				for (uint32_t j = 0; j < face.mNumIndices; j++)
					meshData.Indices.push_back(face.mIndices[j]);
			}

			return meshData;
		}

		std::string ReadAssimpTexturePath(const aiMaterial& assimpMaterial, aiTextureType textureType)
		{
			aiString texturePath;
			if (assimpMaterial.GetTexture(textureType, 0, &texturePath) != AI_SUCCESS)
				return "";

			return texturePath.C_Str();
		}

		Ref<Texture2D> LoadModelTexture(
			const std::string& modelDirectory,
			const std::string& modelName,
			uint32_t materialIndex,
			const std::string& texturePath,
			const std::string& usage
		)
		{
			if (texturePath.empty())
				return nullptr;

			std::filesystem::path resolvedPath = NormalizePathObject(std::filesystem::path(modelDirectory) / texturePath);
			std::string textureName = modelName + ":" + std::to_string(materialIndex) + ":" + usage;
			return ResourceManager::LoadTexture2D(textureName, resolvedPath.string());
		}

		Ref<Material> ConvertAssimpMaterial(
			const aiScene& scene,
			const std::string& modelDirectory,
			const std::string& modelName,
			uint32_t materialIndex,
			const Ref<Shader>& shader
		)
		{
			Ref<Material> material = CreateRef<Material>(shader);
			if (materialIndex >= scene.mNumMaterials)
				return material;

			const aiMaterial& assimpMaterial = *scene.mMaterials[materialIndex];

			aiColor4D diffuseColor;
			if (aiGetMaterialColor(&assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
				material->SetFloat4("u_AlbedoColor", { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a });

			Ref<Texture2D> albedoTexture = LoadModelTexture(
				modelDirectory,
				modelName,
				materialIndex,
				ReadAssimpTexturePath(assimpMaterial, aiTextureType_DIFFUSE),
				"albedo"
			);
			if (albedoTexture)
				material->SetAlbedoTexture(albedoTexture, 0);

			Ref<Texture2D> lightmapTexture = LoadModelTexture(
				modelDirectory,
				modelName,
				materialIndex,
				ReadAssimpTexturePath(assimpMaterial, aiTextureType_LIGHTMAP),
				"lightmap"
			);
			if (lightmapTexture)
				material->SetLightmapTexture(lightmapTexture, 1);

			return material;
		}

		void ProcessAssimpNode(
			const aiScene& scene,
			const aiNode& node,
			const std::string& modelDirectory,
			const std::string& modelName,
			const Ref<Shader>& shader,
			const Ref<Model>& model
		)
		{
			for (uint32_t i = 0; i < node.mNumMeshes; i++)
			{
				const aiMesh& assimpMesh = *scene.mMeshes[node.mMeshes[i]];
				ModelSubmesh submesh;
				submesh.Name = assimpMesh.mName.C_Str();
				submesh.MaterialIndex = assimpMesh.mMaterialIndex;
				submesh.MeshAsset = Mesh::Create(ConvertAssimpMesh(assimpMesh));
				submesh.MaterialAsset = ConvertAssimpMaterial(scene, modelDirectory, modelName, assimpMesh.mMaterialIndex, shader);
				model->AddSubmesh(submesh);
			}

			for (uint32_t i = 0; i < node.mNumChildren; i++)
				ProcessAssimpNode(scene, *node.mChildren[i], modelDirectory, modelName, shader, model);
		}

		Ref<Model> LoadModelWithAssimp(const std::string& name, const std::string& normalizedPath, const Ref<Shader>& shader)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(
				normalizedPath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices |
				aiProcess_ValidateDataStructure
			);

			FT_CORE_ASSERT(scene && scene->mRootNode, "Assimp failed to load model!");

			Ref<Model> model = CreateRef<Model>(normalizedPath);
			std::filesystem::path modelDirectory = std::filesystem::path(normalizedPath).parent_path();
			ProcessAssimpNode(*scene, *scene->mRootNode, modelDirectory.string(), name, shader, model);
			FT_CORE_ASSERT(!model->IsEmpty(), "Assimp model contained no meshes!");
			return model;
		}
#endif
	}

	void ResourceManager::Initialize(const std::string& assetRoot)
	{
		s_AssetRoot = std::filesystem::path(assetRoot).lexically_normal().generic_string();
	}

	Ref<Shader> ResourceManager::LoadShader(const std::string& name, const std::string& relativePath)
	{
		FT_CORE_ASSERT(!name.empty(), "Shader resource name cannot be empty!");

		std::string resolvedPath = NormalizeAssetPath(ResolveAssetPath(relativePath));
		if (HasShader(name))
		{
			FT_CORE_ASSERT(s_ShaderPathAliases[name] == resolvedPath, "Shader name already used for a different path!");
			return GetShader(name);
		}

		for (const auto& [existingName, existingPath] : s_ShaderPathAliases)
		{
			if (existingPath == resolvedPath)
			{
				s_Shaders[name] = s_Shaders[existingName];
				s_ShaderPathAliases[name] = resolvedPath;
				return s_Shaders[name];
			}
		}

		Ref<Shader> shader = Shader::Create(resolvedPath, "");
		s_Shaders[name] = shader;
		s_ShaderPathAliases[name] = resolvedPath;
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
		FT_CORE_ASSERT(!name.empty(), "Texture resource name cannot be empty!");

		std::string resolvedPath = NormalizeAssetPath(ResolveAssetPath(relativePath));
		if (HasTexture2D(name))
		{
			FT_CORE_ASSERT(s_TexturePathAliases[name] == resolvedPath, "Texture name already used for a different path!");
			return GetTexture2D(name);
		}

		for (const auto& [existingName, existingPath] : s_TexturePathAliases)
		{
			if (existingPath == resolvedPath)
			{
				s_Textures[name] = s_Textures[existingName];
				s_TexturePathAliases[name] = resolvedPath;
				return s_Textures[name];
			}
		}

		Ref<Texture2D> texture = Texture2D::Create(resolvedPath);
		s_Textures[name] = texture;
		s_TexturePathAliases[name] = resolvedPath;
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

	Ref<Model> ResourceManager::LoadModel(const std::string& name, const std::string& relativePath, const Ref<Shader>& shader)
	{
		FT_CORE_ASSERT(!name.empty(), "Model resource name cannot be empty!");
		FT_CORE_ASSERT(shader, "Model loading requires a shader for generated materials!");

		std::string resolvedPath = NormalizeAssetPath(ResolveAssetPath(relativePath));
		if (HasModel(name))
		{
			FT_CORE_ASSERT(s_ModelPathAliases[name] == resolvedPath, "Model name already used for a different path!");
			return GetModel(name);
		}

		for (const auto& [existingName, existingPath] : s_ModelPathAliases)
		{
			if (existingPath == resolvedPath)
			{
				s_Models[name] = s_Models[existingName];
				s_ModelPathAliases[name] = resolvedPath;
				return s_Models[name];
			}
		}

#ifdef FT_ENABLE_ASSIMP
		Ref<Model> model = LoadModelWithAssimp(name, resolvedPath, shader);
#else
		FT_CORE_ASSERT(false, "Assimp support is not enabled. Define FT_ENABLE_ASSIMP and link Assimp to load model files.");
		Ref<Model> model = nullptr;
#endif

		s_Models[name] = model;
		s_ModelPathAliases[name] = resolvedPath;
		return model;
	}

	Ref<Model> ResourceManager::GetModel(const std::string& name)
	{
		FT_CORE_ASSERT(HasModel(name), "Model does not exist in ResourceManager!");
		return s_Models[name];
	}

	bool ResourceManager::HasModel(const std::string& name)
	{
		return s_Models.find(name) != s_Models.end();
	}

	std::string ResourceManager::ResolveAssetPath(const std::string& relativePath)
	{
		std::filesystem::path inputPath = relativePath;
		if (inputPath.is_absolute())
			return NormalizeAssetPath(relativePath);

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
			return NormalizeAssetPath(candidate);

		std::string projectDirCandidate = "../" + candidate;
		if (std::filesystem::exists(projectDirCandidate))
			return NormalizeAssetPath(projectDirCandidate);

		std::string outputDirCandidate = "../../../" + candidate;
		if (std::filesystem::exists(outputDirCandidate))
			return NormalizeAssetPath(outputDirCandidate);

		return NormalizeAssetPath(candidate);
	}

	std::string ResourceManager::NormalizeAssetPath(const std::string& path)
	{
		return NormalizePathString(path);
	}
}
