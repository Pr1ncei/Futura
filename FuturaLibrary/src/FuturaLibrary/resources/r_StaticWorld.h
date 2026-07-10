/**
 *  @file r_StaticWorld.h
 *
 *  @brief Declares engine-owned static world data built from imported model surfaces.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/renderer/r_Mesh.h"
#include "FuturaLibrary/resources/r_Model.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <vector>

namespace FuturaLibrary
{
	struct WorldTransform
	{
		glm::mat4 Matrix = glm::mat4(1.0f);
	};

	struct WorldMaterialRef
	{
		std::string Name;
		Ref<Material> MaterialAsset;
		uint32_t SourceMaterialIndex = 0;
	};

	struct WorldSurface
	{
		std::string Name;
		std::string SourceModelPath;
		uint32_t SourceSubmeshIndex = 0;
		uint32_t MaterialIndex = 0;
		Ref<Mesh> MeshAsset;
		WorldTransform Transform;
		AxisAlignedBounds LocalBounds;
		AxisAlignedBounds WorldBounds;
	};

	struct WorldTriangle
	{
		glm::vec3 A = glm::vec3(0.0f);
		glm::vec3 B = glm::vec3(0.0f);
		glm::vec3 C = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		AxisAlignedBounds Bounds;
		uint32_t SourceSurfaceIndex = 0;
	};

	struct WorldRaycastHit
	{
		bool Hit = false;
		float Distance = 0.0f;
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		uint32_t SurfaceIndex = 0;
	};

	struct CollisionQueryStats
	{
		float CollisionTimeMs = 0.0f;
		uint32_t BroadPhaseQueries = 0;
		uint32_t CandidateSurfaces = 0;
		uint32_t CandidateTriangles = 0;
		uint32_t NarrowPhaseTests = 0;
		uint32_t ContactsGenerated = 0;
	};

	struct WorldAccelerationStats
	{
		float CellSize = 0.0f;
		uint32_t OccupiedCells = 0;
		uint32_t IndexedSurfaces = 0;
		uint32_t IndexedTriangles = 0;
	};

	class FT_API StaticWorld
	{
	public:
		StaticWorld() = default;
		explicit StaticWorld(const std::string& sourceName);

		void AddModel(const Ref<Model>& model, const WorldTransform& transform = {});

		const std::string& GetSourceName() const { return m_SourceName; }
		const WorldTransform& GetTransform() const { return m_Transform; }
		const std::vector<WorldSurface>& GetSurfaces() const { return m_Surfaces; }
		const std::vector<WorldMaterialRef>& GetMaterials() const { return m_Materials; }
		const std::vector<WorldTriangle>& GetCollisionTriangles() const { return m_CollisionTriangles; }
		const AxisAlignedBounds& GetLocalBounds() const { return m_LocalBounds; }
		const AxisAlignedBounds& GetWorldBounds() const { return m_WorldBounds; }
		const WorldAccelerationStats& GetAccelerationStats() const { return m_AccelerationStats; }
		bool IsEmpty() const { return m_Surfaces.empty(); }
		bool HasCollisionMesh() const { return !m_CollisionTriangles.empty(); }

		WorldRaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) const;
		glm::vec3 ResolveAABBMovement(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& desiredDelta, CollisionQueryStats* stats = nullptr) const;
		void QuerySurfaces(const AxisAlignedBounds& bounds, std::vector<uint32_t>& candidates) const;

		static Ref<StaticWorld> CreateFromModel(const Ref<Model>& model, const WorldTransform& transform = {});

	private:
		struct SpatialCell
		{
			std::vector<uint32_t> Surfaces;
			std::vector<uint32_t> CollisionTriangles;
		};

		void ExtractCollisionTriangles(const WorldSurface& surface);
		void BuildSpatialGrid();
		void QueryCollisionTriangles(const AxisAlignedBounds& bounds, std::vector<uint32_t>& candidates, CollisionQueryStats* stats = nullptr) const;

		std::string m_SourceName;
		WorldTransform m_Transform;
		std::vector<WorldSurface> m_Surfaces;
		std::vector<WorldMaterialRef> m_Materials;
		std::vector<WorldTriangle> m_CollisionTriangles;
		std::unordered_map<int64_t, SpatialCell> m_SpatialGrid;
		mutable std::vector<uint32_t> m_CollisionQueryCandidates;
		mutable std::vector<uint32_t> m_CollisionQueryMarks;
		mutable std::vector<uint32_t> m_SurfaceQueryMarks;
		mutable uint32_t m_CollisionQueryStamp = 1;
		mutable uint32_t m_SurfaceQueryStamp = 1;
		float m_SpatialGridCellSize = 8.0f;
		WorldAccelerationStats m_AccelerationStats;
		AxisAlignedBounds m_LocalBounds;
		AxisAlignedBounds m_WorldBounds;
	};
}
