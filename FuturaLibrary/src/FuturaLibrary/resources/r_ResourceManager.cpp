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
 *      Last Modified on:    July 07, 2026
 */

#include "pch.h"
#include "r_ResourceManager.h"

#include <cctype>
#include <cmath>
#include <filesystem>
#include <limits>

#ifdef FT_ENABLE_ASSIMP
	#include <assimp/Importer.hpp>
	#include <assimp/material.h>
	#include <assimp/postprocess.h>
	#include <assimp/scene.h>
	#include <assimp/version.h>
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
		std::string Trim(const std::string& value)
		{
			const size_t first = value.find_first_not_of(" \t\r\n");
			if (first == std::string::npos)
				return "";

			const size_t last = value.find_last_not_of(" \t\r\n");
			return value.substr(first, last - first + 1);
		}

		std::string ToLower(std::string value)
		{
			std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character)
			{
				return static_cast<char>(std::tolower(character));
			});

			return value;
		}

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

		bool PathExists(const std::filesystem::path& path)
		{
			std::error_code error;
			return std::filesystem::exists(path, error);
		}

		std::string NormalizePathString(const std::filesystem::path& path)
		{
			return NormalizePathObject(path).generic_string();
		}

#ifdef FT_ENABLE_ASSIMP
		constexpr uint32_t ModelCacheMagic = 0x4C444D46; // FMDL
		constexpr uint32_t ModelCacheFormatVersion = 1;
		constexpr uint32_t EngineMeshFormatVersion = 2;
		constexpr uint64_t MaxCachedStringLength = 1024 * 1024;
		constexpr uint64_t MaxCachedElementCount = 100000000;
		static_assert(std::is_trivially_copyable_v<Vertex>, "Model cache requires Vertex to be block-serializable.");

		struct CachedMaterialData
		{
			std::string Name;
			glm::vec4 AlbedoColor = glm::vec4(1.0f);
			std::string AlbedoTexturePath;
			std::string LightmapTexturePath;
		};

		struct CachedSubmeshData
		{
			std::string Name;
			uint32_t MaterialIndex = 0;
			MeshData Mesh;
			AxisAlignedBounds LocalBounds;
		};

		struct CachedModelData
		{
			std::string SourcePath;
			uint64_t SourceFingerprint = 0;
			std::vector<CachedMaterialData> Materials;
			std::vector<CachedSubmeshData> Submeshes;
		};

		glm::vec3 ToVec3(const aiVector3D& value)
		{
			return { value.x, value.y, value.z };
		}

		glm::vec2 ToVec2(const aiVector3D& value)
		{
			return { value.x, value.y };
		}

		uint32_t GetImporterVersion()
		{
			return (aiGetVersionMajor() << 24) | (aiGetVersionMinor() << 16) | aiGetVersionRevision();
		}

		uint64_t HashString(const std::string& value)
		{
			uint64_t hash = 14695981039346656037ull;
			for (char character : value)
			{
				hash ^= static_cast<unsigned char>(character);
				hash *= 1099511628211ull;
			}

			return hash;
		}

		void HashCombine(uint64_t& hash, uint64_t value)
		{
			hash ^= value + 0x9e3779b97f4a7c15ull + (hash << 6) + (hash >> 2);
		}

		uint64_t GetFileModifiedTime(const std::filesystem::path& path)
		{
			std::error_code error;
			const auto modifiedTime = std::filesystem::last_write_time(path, error);
			if (error)
				return 0;

			return static_cast<uint64_t>(modifiedTime.time_since_epoch().count());
		}

		uint64_t GetFileSize(const std::filesystem::path& path)
		{
			std::error_code error;
			const uintmax_t size = std::filesystem::file_size(path, error);
			if (error)
				return 0;

			return static_cast<uint64_t>(size);
		}

		std::vector<std::filesystem::path> CollectObjMaterialLibraries(const std::string& normalizedPath)
		{
			std::vector<std::filesystem::path> materialLibraries;
			std::filesystem::path modelPath = normalizedPath;
			if (ToLower(modelPath.extension().string()) != ".obj")
				return materialLibraries;

			std::ifstream input(normalizedPath);
			if (!input.is_open())
				return materialLibraries;

			std::filesystem::path modelDirectory = modelPath.parent_path();
			std::string line;
			while (std::getline(input, line))
			{
				std::string trimmedLine = Trim(line);
				if (trimmedLine.empty() || trimmedLine[0] == '#')
					continue;

				// minimal: OBJ mtllib is treated as header metadata; upgrade if assets place mtllib after geometry.
				if (trimmedLine.rfind("v ", 0) == 0 ||
					trimmedLine.rfind("vn ", 0) == 0 ||
					trimmedLine.rfind("vt ", 0) == 0 ||
					trimmedLine.rfind("f ", 0) == 0 ||
					trimmedLine.rfind("o ", 0) == 0 ||
					trimmedLine.rfind("g ", 0) == 0 ||
					trimmedLine.rfind("usemtl ", 0) == 0)
					break;

				if (trimmedLine.rfind("mtllib", 0) != 0)
					continue;
				if (trimmedLine.length() > 6 && !std::isspace(static_cast<unsigned char>(trimmedLine[6])))
					continue;

				std::string materialLibraryList = Trim(trimmedLine.substr(6));
				if (materialLibraryList.empty())
					continue;

				std::stringstream materialLibrariesStream(materialLibraryList);
				std::string materialLibraryPath;
				while (materialLibrariesStream >> materialLibraryPath)
					materialLibraries.push_back(NormalizePathObject(modelDirectory / materialLibraryPath));
			}

			return materialLibraries;
		}

		uint64_t CalculateSourceFingerprint(const std::string& normalizedPath)
		{
			uint64_t fingerprint = HashString(NormalizePathObject(normalizedPath).generic_string());
			auto addFile = [&fingerprint](const std::filesystem::path& path)
			{
				HashCombine(fingerprint, HashString(NormalizePathObject(path).generic_string()));
				HashCombine(fingerprint, GetFileModifiedTime(path));
				HashCombine(fingerprint, GetFileSize(path));
			};

			addFile(normalizedPath);
			for (const std::filesystem::path& materialLibrary : CollectObjMaterialLibraries(normalizedPath))
				addFile(materialLibrary);

			return fingerprint;
		}

		std::filesystem::path GetModelCachePath(const std::string& normalizedPath)
		{
			std::filesystem::path sourcePath = normalizedPath;
			std::stringstream cacheName;
			cacheName << sourcePath.stem().string() << "-" << std::hex << HashString(NormalizePathObject(sourcePath).generic_string()) << ".fmodel";

			return sourcePath.parent_path() / ".futura-cache" / cacheName.str();
		}

		template <typename T>
		bool WriteValue(std::ofstream& output, const T& value)
		{
			output.write(reinterpret_cast<const char*>(&value), sizeof(T));
			return output.good();
		}

		template <typename T>
		bool ReadValue(std::ifstream& input, T& value)
		{
			input.read(reinterpret_cast<char*>(&value), sizeof(T));
			return input.good();
		}

		bool WriteString(std::ofstream& output, const std::string& value)
		{
			const uint64_t length = static_cast<uint64_t>(value.size());
			if (!WriteValue(output, length))
				return false;

			output.write(value.data(), static_cast<std::streamsize>(length));
			return output.good();
		}

		bool ReadString(std::ifstream& input, std::string& value)
		{
			uint64_t length = 0;
			if (!ReadValue(input, length))
				return false;
			if (length > MaxCachedStringLength)
				return false;

			value.resize(static_cast<size_t>(length));
			if (length > 0)
				input.read(value.data(), static_cast<std::streamsize>(length));

			return input.good();
		}

		bool WriteVec2(std::ofstream& output, const glm::vec2& value)
		{
			return WriteValue(output, value.x) && WriteValue(output, value.y);
		}

		bool ReadVec2(std::ifstream& input, glm::vec2& value)
		{
			return ReadValue(input, value.x) && ReadValue(input, value.y);
		}

		bool WriteVec3(std::ofstream& output, const glm::vec3& value)
		{
			return WriteValue(output, value.x) && WriteValue(output, value.y) && WriteValue(output, value.z);
		}

		bool ReadVec3(std::ifstream& input, glm::vec3& value)
		{
			return ReadValue(input, value.x) && ReadValue(input, value.y) && ReadValue(input, value.z);
		}

		bool WriteVec4(std::ofstream& output, const glm::vec4& value)
		{
			return WriteValue(output, value.x) && WriteValue(output, value.y) && WriteValue(output, value.z) && WriteValue(output, value.w);
		}

		bool ReadVec4(std::ifstream& input, glm::vec4& value)
		{
			return ReadValue(input, value.x) && ReadValue(input, value.y) && ReadValue(input, value.z) && ReadValue(input, value.w);
		}

		bool WriteBounds(std::ofstream& output, const AxisAlignedBounds& bounds)
		{
			return WriteValue(output, bounds.IsValid) && WriteVec3(output, bounds.Min) && WriteVec3(output, bounds.Max);
		}

		bool ReadBounds(std::ifstream& input, AxisAlignedBounds& bounds)
		{
			return ReadValue(input, bounds.IsValid) && ReadVec3(input, bounds.Min) && ReadVec3(input, bounds.Max);
		}

		bool WriteVertices(std::ofstream& output, const std::vector<Vertex>& vertices)
		{
			if (vertices.empty())
				return true;

			output.write(
				reinterpret_cast<const char*>(vertices.data()),
				static_cast<std::streamsize>(vertices.size() * sizeof(Vertex))
			);
			return output.good();
		}

		bool ReadVertices(std::ifstream& input, std::vector<Vertex>& vertices)
		{
			if (vertices.empty())
				return true;

			input.read(
				reinterpret_cast<char*>(vertices.data()),
				static_cast<std::streamsize>(vertices.size() * sizeof(Vertex))
			);
			return input.good();
		}

		bool WriteIndices(std::ofstream& output, const std::vector<uint32_t>& indices)
		{
			if (indices.empty())
				return true;

			output.write(
				reinterpret_cast<const char*>(indices.data()),
				static_cast<std::streamsize>(indices.size() * sizeof(uint32_t))
			);
			return output.good();
		}

		bool ReadIndices(std::ifstream& input, std::vector<uint32_t>& indices)
		{
			if (indices.empty())
				return true;

			input.read(
				reinterpret_cast<char*>(indices.data()),
				static_cast<std::streamsize>(indices.size() * sizeof(uint32_t))
			);
			return input.good();
		}

		bool IsFinite(float value)
		{
			return std::isfinite(value);
		}

		bool IsFiniteVec2(const glm::vec2& value)
		{
			return IsFinite(value.x) && IsFinite(value.y);
		}

		bool IsFiniteVec3(const glm::vec3& value)
		{
			return IsFinite(value.x) && IsFinite(value.y) && IsFinite(value.z);
		}

		bool IsFiniteVec4(const glm::vec4& value)
		{
			return IsFinite(value.x) && IsFinite(value.y) && IsFinite(value.z) && IsFinite(value.w);
		}

		bool IsValidBounds(const AxisAlignedBounds& bounds)
		{
			if (!bounds.IsValid)
				return false;

			return IsFiniteVec3(bounds.Min) &&
				IsFiniteVec3(bounds.Max) &&
				bounds.Min.x <= bounds.Max.x &&
				bounds.Min.y <= bounds.Max.y &&
				bounds.Min.z <= bounds.Max.z;
		}

		bool ValidateCachedSubmesh(CachedSubmeshData& submesh, size_t materialCount)
		{
			if (submesh.Mesh.Vertices.empty() || submesh.Mesh.Indices.empty())
				return false;
			if (submesh.Mesh.Indices.size() % 3 != 0)
				return false;
			if (submesh.Mesh.Vertices.size() * sizeof(Vertex) > std::numeric_limits<uint32_t>::max())
				return false;
			if (submesh.Mesh.Indices.size() > std::numeric_limits<uint32_t>::max())
				return false;
			if (submesh.MaterialIndex >= materialCount)
				return false;

			for (const Vertex& vertex : submesh.Mesh.Vertices)
			{
				if (!IsFiniteVec3(vertex.Position) ||
					!IsFiniteVec2(vertex.TexCoord) ||
					!IsFiniteVec3(vertex.Normal) ||
					!IsFiniteVec2(vertex.LightmapTexCoord))
					return false;
			}

			const size_t vertexCount = submesh.Mesh.Vertices.size();
			for (uint32_t index : submesh.Mesh.Indices)
			{
				if (index >= vertexCount)
					return false;
			}

			AxisAlignedBounds calculatedBounds = CalculateMeshBounds(submesh.Mesh.Vertices);
			if (!IsValidBounds(calculatedBounds))
				return false;

			submesh.LocalBounds = calculatedBounds;
			submesh.Mesh.LocalBounds = calculatedBounds;
			return true;
		}

		bool ValidateCachedModelData(CachedModelData& modelData)
		{
			if (modelData.Submeshes.empty())
				return false;

			for (const CachedMaterialData& material : modelData.Materials)
			{
				if (!IsFiniteVec4(material.AlbedoColor))
					return false;
			}

			for (CachedSubmeshData& submesh : modelData.Submeshes)
			{
				if (!ValidateCachedSubmesh(submesh, modelData.Materials.size()))
					return false;
			}

			return true;
		}

		bool SaveModelCache(const std::filesystem::path& cachePath, const CachedModelData& modelData)
		{
			std::error_code error;
			std::filesystem::create_directories(cachePath.parent_path(), error);
			if (error)
			{
				FT_CORE_WARN("Unable to create model cache directory '{0}': {1}", cachePath.parent_path().generic_string(), error.message());
				return false;
			}

			std::ofstream output(cachePath, std::ios::binary | std::ios::trunc);
			if (!output.is_open())
			{
				FT_CORE_WARN("Unable to write model cache '{0}'.", cachePath.generic_string());
				return false;
			}

			const uint32_t materialCount = static_cast<uint32_t>(modelData.Materials.size());
			const uint32_t submeshCount = static_cast<uint32_t>(modelData.Submeshes.size());
			if (!WriteValue(output, ModelCacheMagic) ||
				!WriteValue(output, ModelCacheFormatVersion) ||
				!WriteValue(output, EngineMeshFormatVersion) ||
				!WriteValue(output, GetImporterVersion()) ||
				!WriteValue(output, modelData.SourceFingerprint) ||
				!WriteString(output, modelData.SourcePath) ||
				!WriteValue(output, materialCount) ||
				!WriteValue(output, submeshCount))
				return false;

			for (const CachedMaterialData& material : modelData.Materials)
			{
				if (!WriteString(output, material.Name) ||
					!WriteVec4(output, material.AlbedoColor) ||
					!WriteString(output, material.AlbedoTexturePath) ||
					!WriteString(output, material.LightmapTexturePath))
					return false;
			}

			for (const CachedSubmeshData& submesh : modelData.Submeshes)
			{
				const uint64_t vertexCount = static_cast<uint64_t>(submesh.Mesh.Vertices.size());
				const uint64_t indexCount = static_cast<uint64_t>(submesh.Mesh.Indices.size());
				if (!WriteString(output, submesh.Name) ||
					!WriteValue(output, submesh.MaterialIndex) ||
					!WriteBounds(output, submesh.LocalBounds) ||
					!WriteValue(output, vertexCount) ||
					!WriteValue(output, indexCount))
					return false;

				if (!WriteVertices(output, submesh.Mesh.Vertices) ||
					!WriteIndices(output, submesh.Mesh.Indices))
					return false;
			}

			return output.good();
		}

		bool LoadModelCache(
			const std::filesystem::path& cachePath,
			const std::string& sourcePath,
			uint64_t sourceFingerprint,
			CachedModelData& modelData
		)
		{
			std::ifstream input(cachePath, std::ios::binary);
			if (!input.is_open())
				return false;

			uint32_t magic = 0;
			uint32_t cacheFormatVersion = 0;
			uint32_t meshFormatVersion = 0;
			uint32_t importerVersion = 0;
			uint64_t cachedSourceFingerprint = 0;
			std::string cachedSourcePath;
			uint32_t materialCount = 0;
			uint32_t submeshCount = 0;

			if (!ReadValue(input, magic) ||
				!ReadValue(input, cacheFormatVersion) ||
				!ReadValue(input, meshFormatVersion) ||
				!ReadValue(input, importerVersion) ||
				!ReadValue(input, cachedSourceFingerprint) ||
				!ReadString(input, cachedSourcePath) ||
				!ReadValue(input, materialCount) ||
				!ReadValue(input, submeshCount))
				return false;

			if (magic != ModelCacheMagic ||
				cacheFormatVersion != ModelCacheFormatVersion ||
				meshFormatVersion != EngineMeshFormatVersion ||
				importerVersion != GetImporterVersion() ||
				cachedSourceFingerprint != sourceFingerprint ||
				cachedSourcePath != sourcePath)
				return false;
			if (materialCount > MaxCachedElementCount || submeshCount > MaxCachedElementCount)
				return false;

			modelData = {};
			modelData.SourcePath = cachedSourcePath;
			modelData.SourceFingerprint = cachedSourceFingerprint;
			modelData.Materials.resize(materialCount);
			modelData.Submeshes.resize(submeshCount);

			for (CachedMaterialData& material : modelData.Materials)
			{
				if (!ReadString(input, material.Name) ||
					!ReadVec4(input, material.AlbedoColor) ||
					!ReadString(input, material.AlbedoTexturePath) ||
					!ReadString(input, material.LightmapTexturePath))
					return false;
			}

			for (CachedSubmeshData& submesh : modelData.Submeshes)
			{
				uint64_t vertexCount = 0;
				uint64_t indexCount = 0;
				if (!ReadString(input, submesh.Name) ||
					!ReadValue(input, submesh.MaterialIndex) ||
					!ReadBounds(input, submesh.LocalBounds) ||
					!ReadValue(input, vertexCount) ||
					!ReadValue(input, indexCount))
					return false;
				if (vertexCount > MaxCachedElementCount || indexCount > MaxCachedElementCount)
					return false;

				submesh.Mesh.Vertices.resize(static_cast<size_t>(vertexCount));
				submesh.Mesh.Indices.resize(static_cast<size_t>(indexCount));

				if (!ReadVertices(input, submesh.Mesh.Vertices) ||
					!ReadIndices(input, submesh.Mesh.Indices))
					return false;
			}

			return input.good() && ValidateCachedModelData(modelData);
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

		std::string ReadAssimpMaterialName(const aiMaterial& assimpMaterial)
		{
			aiString materialName;
			if (assimpMaterial.Get(AI_MATKEY_NAME, materialName) != AI_SUCCESS)
				return "<unnamed>";

			return materialName.C_Str();
		}

		void WarnMissingObjMaterialLibraries(const std::string& modelName, const std::string& normalizedPath)
		{
			std::filesystem::path modelPath = normalizedPath;
			for (const std::filesystem::path& materialLibraryPath : CollectObjMaterialLibraries(normalizedPath))
			{
				if (!PathExists(materialLibraryPath))
				{
					FT_CORE_WARN(
						"Model '{0}' references missing material library '{1}' from OBJ '{2}'. Materials from that library will use fallback values.",
						modelName,
						materialLibraryPath.generic_string(),
						NormalizePathObject(modelPath).generic_string()
					);
				}
			}
		}

		Ref<Texture2D> LoadModelTexture(
			const std::string& modelDirectory,
			const std::string& modelName,
			uint32_t materialIndex,
			const std::string& materialName,
			const std::string& texturePath,
			const std::string& usage
		)
		{
			if (texturePath.empty())
				return nullptr;

			std::filesystem::path resolvedPath = NormalizePathObject(std::filesystem::path(modelDirectory) / texturePath);
			if (!PathExists(resolvedPath))
			{
				FT_CORE_WARN(
					"Model '{0}' material '{1}' (index {2}) references missing {3} texture '{4}' resolved to '{5}'. Rendering will use material fallback values.",
					modelName,
					materialName,
					materialIndex,
					usage,
					texturePath,
					resolvedPath.generic_string()
				);
				return nullptr;
			}

			std::string textureName = modelName + ":" + std::to_string(materialIndex) + ":" + usage;
			return ResourceManager::LoadTexture2D(textureName, resolvedPath.string());
		}

		Ref<Material> CreateMaterialFromCachedData(
			const CachedMaterialData& materialData,
			const std::string& modelDirectory,
			const std::string& modelName,
			uint32_t materialIndex,
			const Ref<Shader>& shader
		)
		{
			Ref<Material> material = CreateRef<Material>(shader);
			material->SetFloat4("u_AlbedoColor", materialData.AlbedoColor);

			Ref<Texture2D> albedoTexture = LoadModelTexture(
				modelDirectory,
				modelName,
				materialIndex,
				materialData.Name,
				materialData.AlbedoTexturePath,
				"albedo"
			);
			if (albedoTexture)
				material->SetAlbedoTexture(albedoTexture, 0);

			Ref<Texture2D> lightmapTexture = LoadModelTexture(
				modelDirectory,
				modelName,
				materialIndex,
				materialData.Name,
				materialData.LightmapTexturePath,
				"lightmap"
			);
			if (lightmapTexture)
				material->SetLightmapTexture(lightmapTexture, 1);

			return material;
		}

		CachedMaterialData ConvertAssimpMaterialData(const aiMaterial& assimpMaterial)
		{
			CachedMaterialData materialData;
			materialData.Name = ReadAssimpMaterialName(assimpMaterial);
			materialData.AlbedoTexturePath = ReadAssimpTexturePath(assimpMaterial, aiTextureType_DIFFUSE);
			materialData.LightmapTexturePath = ReadAssimpTexturePath(assimpMaterial, aiTextureType_LIGHTMAP);

			aiColor4D diffuseColor;
			if (aiGetMaterialColor(&assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
				materialData.AlbedoColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };

			return materialData;
		}

		void ProcessAssimpNode(
			const aiScene& scene,
			const aiNode& node,
			CachedModelData& modelData
		)
		{
			for (uint32_t i = 0; i < node.mNumMeshes; i++)
			{
				const aiMesh& assimpMesh = *scene.mMeshes[node.mMeshes[i]];
				CachedSubmeshData submesh;
				submesh.Name = assimpMesh.mName.C_Str();
				submesh.MaterialIndex = assimpMesh.mMaterialIndex;
				submesh.Mesh = ConvertAssimpMesh(assimpMesh);
				submesh.LocalBounds = CalculateMeshBounds(submesh.Mesh.Vertices);
				submesh.Mesh.LocalBounds = submesh.LocalBounds;
				modelData.Submeshes.push_back(submesh);
			}

			for (uint32_t i = 0; i < node.mNumChildren; i++)
				ProcessAssimpNode(scene, *node.mChildren[i], modelData);
		}

		CachedModelData ConvertAssimpSceneToCachedData(
			const aiScene& scene,
			const std::string& sourcePath,
			uint64_t sourceFingerprint
		)
		{
			CachedModelData modelData;
			modelData.SourcePath = sourcePath;
			modelData.SourceFingerprint = sourceFingerprint;
			modelData.Materials.reserve(scene.mNumMaterials);
			modelData.Submeshes.reserve(scene.mNumMeshes);

			for (uint32_t i = 0; i < scene.mNumMaterials; i++)
				modelData.Materials.push_back(ConvertAssimpMaterialData(*scene.mMaterials[i]));

			ProcessAssimpNode(scene, *scene.mRootNode, modelData);
			return modelData;
		}

		Ref<Model> CreateModelFromCachedData(
			const CachedModelData& modelData,
			const std::string& modelName,
			const Ref<Shader>& shader
		)
		{
			Ref<Model> model = CreateRef<Model>(modelData.SourcePath);
			std::filesystem::path modelDirectory = std::filesystem::path(modelData.SourcePath).parent_path();

			for (const CachedSubmeshData& cachedSubmesh : modelData.Submeshes)
			{
				ModelSubmesh submesh;
				submesh.Name = cachedSubmesh.Name;
				submesh.MaterialIndex = cachedSubmesh.MaterialIndex;
				MeshData meshData = cachedSubmesh.Mesh;
				meshData.LocalBounds = cachedSubmesh.LocalBounds;
				submesh.MeshAsset = Mesh::Create(meshData);
				submesh.LocalBounds = cachedSubmesh.LocalBounds;

				if (cachedSubmesh.MaterialIndex < modelData.Materials.size())
				{
					submesh.MaterialAsset = CreateMaterialFromCachedData(
						modelData.Materials[cachedSubmesh.MaterialIndex],
						modelDirectory.string(),
						modelName,
						cachedSubmesh.MaterialIndex,
						shader
					);
				}
				else
				{
					submesh.MaterialAsset = CreateRef<Material>(shader);
				}

				model->AddSubmesh(submesh);
			}

			return model;
		}

		Ref<Model> LoadModelWithAssimp(const std::string& name, const std::string& normalizedPath, const Ref<Shader>& shader)
		{
			const std::string sourcePath = NormalizePathObject(normalizedPath).generic_string();
			const uint64_t sourceFingerprint = CalculateSourceFingerprint(normalizedPath);
			const std::filesystem::path cachePath = GetModelCachePath(normalizedPath);

			CachedModelData modelData;
			if (LoadModelCache(cachePath, sourcePath, sourceFingerprint, modelData))
			{
				FT_CORE_INFO("Loaded model '{0}' from cache '{1}'.", name, cachePath.generic_string());
				return CreateModelFromCachedData(modelData, name, shader);
			}

			WarnMissingObjMaterialLibraries(name, normalizedPath);
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(
				normalizedPath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices |
				aiProcess_ValidateDataStructure
			);

			FT_CORE_ASSERT(scene && scene->mRootNode, "Assimp failed to load model!");

			modelData = ConvertAssimpSceneToCachedData(*scene, sourcePath, sourceFingerprint);
			FT_CORE_ASSERT(!modelData.Submeshes.empty(), "Assimp model contained no meshes!");
			if (SaveModelCache(cachePath, modelData))
				FT_CORE_INFO("Wrote model cache for '{0}' to '{1}'.", name, cachePath.generic_string());

			Ref<Model> model = CreateModelFromCachedData(modelData, name, shader);
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
