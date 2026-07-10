/**
 *  @file r_StaticWorldRenderer.h
 *
 *  @brief Declares the renderer path for static world surface submission.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/renderer/r_Mesh.h"

#include <glm/glm.hpp>

namespace FuturaLibrary
{
	class StaticWorld;

	struct StaticWorldSurfaceSubmission
	{
		Ref<Material> Material;
		Ref<Mesh> Mesh;
		glm::mat4 Transform = glm::mat4(1.0f);
		uint32_t SourceSurfaceIndex = 0;
	};

	class FT_API StaticWorldRenderer
	{
	public:
		static void Submit(const Ref<StaticWorld>& world, const Ref<Material>& fallbackMaterial, const glm::mat4& viewProjection);
	};
}
