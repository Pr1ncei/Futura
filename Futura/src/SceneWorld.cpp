/**
 *  @file SceneWorld.cpp
 *
 *  @brief Implements scene data loading for preview content.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#include "SceneWorld.h"

#include "FuturaLibrary/resources/r_ResourceManager.h"
#include "FuturaLibrary/renderer/r_Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <unordered_map>

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

	bool ReadFloat(const std::unordered_map<std::string, std::string>& values, const std::string& key, float& output)
	{
		const auto value = values.find(key);
		if (value == values.end())
			return false;

		std::stringstream stream(value->second);
		stream >> output;
		return !stream.fail();
	}

	bool ReadVec3(const std::unordered_map<std::string, std::string>& values, const std::string& key, glm::vec3& output)
	{
		const auto value = values.find(key);
		if (value == values.end())
			return false;

		std::stringstream stream(value->second);
		char commaA = '\0';
		char commaB = '\0';
		stream >> output.x >> commaA >> output.y >> commaB >> output.z;
		return !stream.fail() && commaA == ',' && commaB == ',';
	}

	std::unordered_map<std::string, std::string> LoadKeyValueFile(const std::string& path)
	{
		std::unordered_map<std::string, std::string> values;
		std::ifstream input(path);
		if (!input.is_open())
		{
			FT_CORE_WARN("Unable to open scene file '{0}'.", path);
			return values;
		}

		std::string line;
		while (std::getline(input, line))
		{
			std::string trimmedLine = Trim(line);
			if (trimmedLine.empty() || trimmedLine[0] == '#')
				continue;

			const size_t separator = trimmedLine.find('=');
			if (separator == std::string::npos)
			{
				FT_CORE_WARN("Ignoring malformed scene line in '{0}': {1}", path, trimmedLine);
				continue;
			}

			std::string key = Trim(trimmedLine.substr(0, separator));
			std::string value = Trim(trimmedLine.substr(separator + 1));
			if (!key.empty())
				values[key] = value;
		}

		return values;
	}
}

bool SceneWorld::LoadPreviewScene(const std::string& scenePath, const FuturaLibrary::Ref<FuturaLibrary::Shader>& shader)
{
	FT_CORE_ASSERT(shader, "SceneWorld preview loading requires a shader!");

	m_ModelInstances.clear();

	const std::string resolvedScenePath = FuturaLibrary::ResourceManager::ResolveAssetPath(scenePath);
	std::unordered_map<std::string, std::string> values = LoadKeyValueFile(resolvedScenePath);
	if (values.empty())
		return false;

	const auto modelName = values.find("model_name");
	const auto modelPath = values.find("model_path");
	if (modelName == values.end() || modelPath == values.end())
	{
		FT_CORE_WARN("Scene file '{0}' must define model_name and model_path.", resolvedScenePath);
		return false;
	}

	float scale = 1.0f;
	glm::vec3 offset = glm::vec3(0.0f);
	if (!ReadFloat(values, "preview_scale", scale))
		FT_CORE_WARN("Scene file '{0}' is missing preview_scale. Using 1.0.", resolvedScenePath);
	if (!ReadVec3(values, "preview_offset", offset))
		FT_CORE_WARN("Scene file '{0}' is missing preview_offset. Using 0,0,0.", resolvedScenePath);

	ModelInstance instance;
	instance.ModelAsset = FuturaLibrary::ResourceManager::LoadModel(modelName->second, modelPath->second, shader);
	instance.Transform = glm::mat4(1.0f);
	instance.Transform = glm::scale(instance.Transform, glm::vec3(scale));
	instance.Transform = glm::translate(instance.Transform, offset);

	m_ModelInstances.push_back(instance);
	return true;
}

void SceneWorld::Submit(const FuturaLibrary::Ref<FuturaLibrary::Material>& fallbackMaterial) const
{
	for (const ModelInstance& instance : m_ModelInstances)
	{
		if (!instance.ModelAsset || instance.ModelAsset->IsEmpty())
			continue;

		for (const FuturaLibrary::ModelSubmesh& submesh : instance.ModelAsset->GetSubmeshes())
			FuturaLibrary::Renderer::Submit({ submesh.MaterialAsset ? submesh.MaterialAsset : fallbackMaterial, submesh.MeshAsset, instance.Transform });
	}
}
