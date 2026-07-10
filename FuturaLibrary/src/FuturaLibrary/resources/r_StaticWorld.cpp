#include "pch.h"
#include "r_StaticWorld.h"

#include <glm/gtx/norm.hpp>

#include <cmath>
#include <limits>

namespace FuturaLibrary
{
	namespace
	{
		constexpr float RayEpsilon = 0.000001f;

		struct GridCoord
		{
			int32_t X = 0;
			int32_t Y = 0;
			int32_t Z = 0;
		};

		void Encapsulate(AxisAlignedBounds& target, const AxisAlignedBounds& source)
		{
			if (!source.IsValid)
				return;

			if (!target.IsValid)
			{
				target = source;
				return;
			}

			target.Min.x = std::min(target.Min.x, source.Min.x);
			target.Min.y = std::min(target.Min.y, source.Min.y);
			target.Min.z = std::min(target.Min.z, source.Min.z);
			target.Max.x = std::max(target.Max.x, source.Max.x);
			target.Max.y = std::max(target.Max.y, source.Max.y);
			target.Max.z = std::max(target.Max.z, source.Max.z);
		}

		AxisAlignedBounds TransformBounds(const AxisAlignedBounds& bounds, const glm::mat4& transform)
		{
			AxisAlignedBounds transformedBounds;
			if (!bounds.IsValid)
				return transformedBounds;

			const glm::vec3 corners[] =
			{
				{ bounds.Min.x, bounds.Min.y, bounds.Min.z },
				{ bounds.Max.x, bounds.Min.y, bounds.Min.z },
				{ bounds.Min.x, bounds.Max.y, bounds.Min.z },
				{ bounds.Max.x, bounds.Max.y, bounds.Min.z },
				{ bounds.Min.x, bounds.Min.y, bounds.Max.z },
				{ bounds.Max.x, bounds.Min.y, bounds.Max.z },
				{ bounds.Min.x, bounds.Max.y, bounds.Max.z },
				{ bounds.Max.x, bounds.Max.y, bounds.Max.z }
			};

			for (const glm::vec3& corner : corners)
			{
				const glm::vec4 transformedCorner = transform * glm::vec4(corner, 1.0f);
				AxisAlignedBounds cornerBounds;
				cornerBounds.Min = glm::vec3(transformedCorner);
				cornerBounds.Max = cornerBounds.Min;
				cornerBounds.IsValid = true;
				Encapsulate(transformedBounds, cornerBounds);
			}

			return transformedBounds;
		}

		AxisAlignedBounds CalculateTriangleBounds(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		{
			AxisAlignedBounds bounds;
			bounds.Min = glm::min(a, glm::min(b, c));
			bounds.Max = glm::max(a, glm::max(b, c));
			bounds.IsValid = true;
			return bounds;
		}

		bool BoundsOverlap(const AxisAlignedBounds& a, const AxisAlignedBounds& b)
		{
			if (!a.IsValid || !b.IsValid)
				return false;

			return a.Min.x <= b.Max.x && a.Max.x >= b.Min.x &&
				a.Min.y <= b.Max.y && a.Max.y >= b.Min.y &&
				a.Min.z <= b.Max.z && a.Max.z >= b.Min.z;
		}

		AxisAlignedBounds MakeAABB(const glm::vec3& center, const glm::vec3& halfExtents)
		{
			AxisAlignedBounds bounds;
			bounds.Min = center - halfExtents;
			bounds.Max = center + halfExtents;
			bounds.IsValid = true;
			return bounds;
		}

		bool RayIntersectsTriangle(
			const glm::vec3& origin,
			const glm::vec3& direction,
			const WorldTriangle& triangle,
			float maxDistance,
			float& distance)
		{
			const glm::vec3 edge1 = triangle.B - triangle.A;
			const glm::vec3 edge2 = triangle.C - triangle.A;
			const glm::vec3 p = glm::cross(direction, edge2);
			const float determinant = glm::dot(edge1, p);
			if (std::abs(determinant) < RayEpsilon)
				return false;

			const float inverseDeterminant = 1.0f / determinant;
			const glm::vec3 t = origin - triangle.A;
			const float u = glm::dot(t, p) * inverseDeterminant;
			if (u < 0.0f || u > 1.0f)
				return false;

			const glm::vec3 q = glm::cross(t, edge1);
			const float v = glm::dot(direction, q) * inverseDeterminant;
			if (v < 0.0f || u + v > 1.0f)
				return false;

			distance = glm::dot(edge2, q) * inverseDeterminant;
			return distance >= 0.0f && distance <= maxDistance;
		}

		bool AABBOverlapsAnyTriangleBounds(
			const AxisAlignedBounds& bounds,
			const std::vector<WorldTriangle>& triangles,
			const std::vector<uint32_t>& candidates,
			CollisionQueryStats* stats)
		{
			for (uint32_t triangleIndex : candidates)
			{
				if (triangleIndex >= triangles.size())
					continue;

				if (stats)
					stats->NarrowPhaseTests++;

				const WorldTriangle& triangle = triangles[triangleIndex];
				if (BoundsOverlap(bounds, triangle.Bounds))
				{
					if (stats)
						stats->ContactsGenerated++;
					return true;
				}
			}

			return false;
		}

		GridCoord ToGridCoord(const glm::vec3& point, float cellSize)
		{
			return {
				static_cast<int32_t>(std::floor(point.x / cellSize)),
				static_cast<int32_t>(std::floor(point.y / cellSize)),
				static_cast<int32_t>(std::floor(point.z / cellSize))
			};
		}

		int64_t HashGridCoord(const GridCoord& coord)
		{
			const int64_t x = static_cast<int64_t>(coord.X) & 0x1fffff;
			const int64_t y = static_cast<int64_t>(coord.Y) & 0x1fffff;
			const int64_t z = static_cast<int64_t>(coord.Z) & 0x1fffff;
			return x | (y << 21) | (z << 42);
		}
	}

	StaticWorld::StaticWorld(const std::string& sourceName)
		: m_SourceName(sourceName)
	{
	}

	void StaticWorld::AddModel(const Ref<Model>& model, const WorldTransform& transform)
	{
		FT_CORE_ASSERT(model, "StaticWorld requires a model!");

		if (m_SourceName.empty())
			m_SourceName = model->GetSourcePath();
		m_Transform = transform;

		const std::vector<ModelSubmesh>& submeshes = model->GetSubmeshes();
		for (size_t i = 0; i < submeshes.size(); i++)
		{
			const ModelSubmesh& submesh = submeshes[i];
			WorldMaterialRef materialRef;
			materialRef.Name = submesh.Name;
			materialRef.MaterialAsset = submesh.MaterialAsset;
			materialRef.SourceMaterialIndex = submesh.MaterialIndex;
			const uint32_t materialIndex = static_cast<uint32_t>(m_Materials.size());
			m_Materials.push_back(materialRef);

			WorldSurface surface;
			surface.Name = submesh.Name;
			surface.SourceModelPath = model->GetSourcePath();
			surface.SourceSubmeshIndex = static_cast<uint32_t>(i);
			surface.MaterialIndex = materialIndex;
			surface.MeshAsset = submesh.MeshAsset;
			surface.Transform = transform;
			surface.LocalBounds = submesh.LocalBounds.IsValid ? submesh.LocalBounds : submesh.MeshAsset->GetLocalBounds();
			surface.WorldBounds = TransformBounds(surface.LocalBounds, transform.Matrix);

			Encapsulate(m_LocalBounds, surface.LocalBounds);
			Encapsulate(m_WorldBounds, surface.WorldBounds);
			m_Surfaces.push_back(surface);
			ExtractCollisionTriangles(m_Surfaces.back());
		}

		BuildCollisionGrid();
	}

	void StaticWorld::ExtractCollisionTriangles(const WorldSurface& surface)
	{
		if (!surface.MeshAsset)
			return;

		const std::vector<Vertex>& vertices = surface.MeshAsset->GetVertices();
		const std::vector<uint32_t>& indices = surface.MeshAsset->GetIndices();
		if (vertices.empty() || indices.size() < 3)
			return;

		for (size_t i = 0; i + 2 < indices.size(); i += 3)
		{
			const uint32_t indexA = indices[i];
			const uint32_t indexB = indices[i + 1];
			const uint32_t indexC = indices[i + 2];
			if (indexA >= vertices.size() || indexB >= vertices.size() || indexC >= vertices.size())
				continue;

			const glm::vec3 a = glm::vec3(surface.Transform.Matrix * glm::vec4(vertices[indexA].Position, 1.0f));
			const glm::vec3 b = glm::vec3(surface.Transform.Matrix * glm::vec4(vertices[indexB].Position, 1.0f));
			const glm::vec3 c = glm::vec3(surface.Transform.Matrix * glm::vec4(vertices[indexC].Position, 1.0f));
			const glm::vec3 normal = glm::cross(b - a, c - a);
			if (glm::length2(normal) <= 0.0f)
				continue;

			WorldTriangle triangle;
			triangle.A = a;
			triangle.B = b;
			triangle.C = c;
			triangle.Normal = glm::normalize(normal);
			triangle.Bounds = CalculateTriangleBounds(a, b, c);
			triangle.SourceSurfaceIndex = static_cast<uint32_t>(m_Surfaces.size() - 1);
			m_CollisionTriangles.push_back(triangle);
		}
	}

	void StaticWorld::BuildCollisionGrid()
	{
		m_CollisionGrid.clear();
		m_CollisionQueryMarks.assign(m_CollisionTriangles.size(), 0);
		m_SurfaceQueryMarks.assign(m_Surfaces.size(), 0);
		m_CollisionQueryCandidates.clear();
		m_CollisionQueryCandidates.reserve(512);
		m_CollisionQueryStamp = 1;
		m_SurfaceQueryStamp = 1;

		if (m_CollisionTriangles.empty())
			return;

		for (uint32_t triangleIndex = 0; triangleIndex < m_CollisionTriangles.size(); triangleIndex++)
		{
			const WorldTriangle& triangle = m_CollisionTriangles[triangleIndex];
			const GridCoord minCoord = ToGridCoord(triangle.Bounds.Min, m_CollisionGridCellSize);
			const GridCoord maxCoord = ToGridCoord(triangle.Bounds.Max, m_CollisionGridCellSize);

			for (int32_t z = minCoord.Z; z <= maxCoord.Z; z++)
			{
				for (int32_t y = minCoord.Y; y <= maxCoord.Y; y++)
				{
					for (int32_t x = minCoord.X; x <= maxCoord.X; x++)
						m_CollisionGrid[HashGridCoord({ x, y, z })].push_back(triangleIndex);
				}
			}
		}

		FT_CORE_INFO(
			"Built static collision grid for '{0}': {1} triangles, {2} occupied cells.",
			m_SourceName,
			m_CollisionTriangles.size(),
			m_CollisionGrid.size()
		);
	}

	void StaticWorld::QueryCollisionGrid(const AxisAlignedBounds& bounds, std::vector<uint32_t>& candidates, CollisionQueryStats* stats) const
	{
		candidates.clear();
		if (!bounds.IsValid || m_CollisionGrid.empty())
			return;

		if (stats)
			stats->BroadPhaseQueries++;

		if (m_CollisionQueryStamp == 0)
		{
			std::fill(m_CollisionQueryMarks.begin(), m_CollisionQueryMarks.end(), 0);
			m_CollisionQueryStamp = 1;
		}
		if (m_SurfaceQueryStamp == 0)
		{
			std::fill(m_SurfaceQueryMarks.begin(), m_SurfaceQueryMarks.end(), 0);
			m_SurfaceQueryStamp = 1;
		}

		uint32_t candidateSurfaces = 0;
		const GridCoord minCoord = ToGridCoord(bounds.Min, m_CollisionGridCellSize);
		const GridCoord maxCoord = ToGridCoord(bounds.Max, m_CollisionGridCellSize);
		for (int32_t z = minCoord.Z; z <= maxCoord.Z; z++)
		{
			for (int32_t y = minCoord.Y; y <= maxCoord.Y; y++)
			{
				for (int32_t x = minCoord.X; x <= maxCoord.X; x++)
				{
					const auto cell = m_CollisionGrid.find(HashGridCoord({ x, y, z }));
					if (cell == m_CollisionGrid.end())
						continue;

					for (uint32_t triangleIndex : cell->second)
					{
						if (triangleIndex >= m_CollisionQueryMarks.size() ||
							m_CollisionQueryMarks[triangleIndex] == m_CollisionQueryStamp)
							continue;

						m_CollisionQueryMarks[triangleIndex] = m_CollisionQueryStamp;
						candidates.push_back(triangleIndex);

						const uint32_t surfaceIndex = m_CollisionTriangles[triangleIndex].SourceSurfaceIndex;
						if (surfaceIndex < m_SurfaceQueryMarks.size() &&
							m_SurfaceQueryMarks[surfaceIndex] != m_SurfaceQueryStamp)
						{
							m_SurfaceQueryMarks[surfaceIndex] = m_SurfaceQueryStamp;
							candidateSurfaces++;
						}
					}
				}
			}
		}

		m_CollisionQueryStamp++;
		m_SurfaceQueryStamp++;
		if (stats)
		{
			stats->CandidateTriangles += static_cast<uint32_t>(candidates.size());
			stats->CandidateSurfaces += candidateSurfaces;
		}
	}

	WorldRaycastHit StaticWorld::Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) const
	{
		WorldRaycastHit closestHit;
		if (glm::length2(direction) <= 0.0f || maxDistance <= 0.0f)
			return closestHit;

		const glm::vec3 rayDirection = glm::normalize(direction);
		AxisAlignedBounds rayBounds;
		rayBounds.Min = glm::min(origin, origin + rayDirection * maxDistance);
		rayBounds.Max = glm::max(origin, origin + rayDirection * maxDistance);
		rayBounds.IsValid = true;
		QueryCollisionGrid(rayBounds, m_CollisionQueryCandidates);

		float closestDistance = maxDistance;
		for (uint32_t triangleIndex : m_CollisionQueryCandidates)
		{
			const WorldTriangle& triangle = m_CollisionTriangles[triangleIndex];
			float distance = 0.0f;
			if (!RayIntersectsTriangle(origin, rayDirection, triangle, closestDistance, distance))
				continue;

			closestDistance = distance;
			closestHit.Hit = true;
			closestHit.Distance = distance;
			closestHit.Position = origin + rayDirection * distance;
			closestHit.Normal = triangle.Normal;
			closestHit.SurfaceIndex = triangle.SourceSurfaceIndex;
		}

		return closestHit;
	}

	glm::vec3 StaticWorld::ResolveAABBMovement(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& desiredDelta, CollisionQueryStats* stats) const
	{
		glm::vec3 resolvedCenter = center;
		glm::vec3 resolvedDelta = glm::vec3(0.0f);

		for (int axis = 0; axis < 3; axis++)
		{
			glm::vec3 axisDelta = glm::vec3(0.0f);
			axisDelta[axis] = desiredDelta[axis];
			if (axisDelta[axis] == 0.0f)
				continue;

			const glm::vec3 candidateCenter = resolvedCenter + axisDelta;
			const AxisAlignedBounds candidateBounds = MakeAABB(candidateCenter, halfExtents);
			QueryCollisionGrid(candidateBounds, m_CollisionQueryCandidates, stats);
			if (AABBOverlapsAnyTriangleBounds(candidateBounds, m_CollisionTriangles, m_CollisionQueryCandidates, stats))
				continue;

			resolvedCenter = candidateCenter;
			resolvedDelta += axisDelta;
		}

		return resolvedDelta;
	}

	Ref<StaticWorld> StaticWorld::CreateFromModel(const Ref<Model>& model, const WorldTransform& transform)
	{
		Ref<StaticWorld> world = CreateRef<StaticWorld>(model ? model->GetSourcePath() : "");
		if (model)
			world->AddModel(model, transform);

		return world;
	}
}
