/**
 *  @file SceneWorld.h
 *
 *  @brief Declares a small scene-loading entry point for preview content.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/resources/r_Model.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class SceneWorld
{
public:
	bool LoadPreviewScene(const std::string& scenePath, const FuturaLibrary::Ref<FuturaLibrary::Shader>& shader);
	void Submit(const FuturaLibrary::Ref<FuturaLibrary::Material>& fallbackMaterial) const;

	bool IsEmpty() const { return m_ModelInstances.empty(); }

private:
	struct ModelInstance
	{
		FuturaLibrary::Ref<FuturaLibrary::Model> ModelAsset;
		glm::mat4 Transform = glm::mat4(1.0f);
	};

	std::vector<ModelInstance> m_ModelInstances;
};
