/**
 *  @file r_DebugRenderer.h
 *
 *  @brief Declares simple debug drawing helpers for lines, bounds, planes, and frustums.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 05, 2026
 *      Last Modified on:    July 05, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_Shader.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	struct DebugDrawStats
	{
		uint32_t LineCount = 0;
		uint32_t VertexCount = 0;
		uint32_t FlushCount = 0;
	};

	class FT_API DebugRenderer
	{
	public:
		static void Initialize(const Ref<Shader>& shader, uint32_t maxLines = 65536);
		static void Shutdown();

		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene();

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
		static void DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color);
		static void DrawBox(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color);
		static void DrawPlane(const glm::vec3& center, const glm::vec3& normal, float halfExtent, const glm::vec4& color);
		static void DrawFrustum(const glm::mat4& inverseViewProjection, const glm::vec4& color);

		static const DebugDrawStats& GetStats();
		static bool IsInitialized();

	private:
		static void Flush();
	};
}
