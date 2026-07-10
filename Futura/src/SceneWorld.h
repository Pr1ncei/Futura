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
#include "FuturaLibrary/renderer/r_DebugRenderer.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/resources/r_StaticWorld.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class SceneWorld
{
public:
	bool LoadPreviewScene(const std::string& scenePath, const FuturaLibrary::Ref<FuturaLibrary::Shader>& shader);
	void Submit(const FuturaLibrary::Ref<FuturaLibrary::Material>& fallbackMaterial) const;
	void DrawDebug(const FuturaLibrary::DebugWorldDrawSettings& settings) const;
	void SelectNextSurface();
	FuturaLibrary::WorldRaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) const;
	glm::vec3 ResolveCameraMovement(const glm::vec3& cameraPosition, const glm::vec3& desiredDelta) const;

	bool IsEmpty() const { return m_StaticWorlds.empty(); }
	bool HasPlanes() const;
	uint32_t GetSelectedSurfaceIndex() const { return m_SelectedSurfaceIndex; }
	const FuturaLibrary::CollisionQueryStats& GetCollisionStats() const { return m_CollisionStats; }

private:
	uint32_t GetSurfaceCount() const;

	std::vector<FuturaLibrary::Ref<FuturaLibrary::StaticWorld>> m_StaticWorlds;
	mutable FuturaLibrary::CollisionQueryStats m_CollisionStats;
	uint32_t m_SelectedSurfaceIndex = 0;
};
