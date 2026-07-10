#include "pch.h"
#include "r_StaticWorldRenderer.h"

#include "FuturaLibrary/renderer/r_Renderer.h"
#include "FuturaLibrary/resources/r_StaticWorld.h"

namespace FuturaLibrary
{
	namespace
	{
		struct FrustumPlane
		{
			glm::vec3 Normal = glm::vec3(0.0f);
			float Distance = 0.0f;
		};

		struct Frustum
		{
			FrustumPlane Planes[6];
		};

		glm::vec4 GetMatrixRow(const glm::mat4& matrix, uint32_t row)
		{
			return { matrix[0][row], matrix[1][row], matrix[2][row], matrix[3][row] };
		}

		FrustumPlane NormalizePlane(const glm::vec4& plane)
		{
			const glm::vec3 normal = glm::vec3(plane);
			const float length = glm::length(normal);
			if (length == 0.0f)
				return {};

			return { normal / length, plane.w / length };
		}

		Frustum ExtractFrustum(const glm::mat4& viewProjection)
		{
			const glm::vec4 row0 = GetMatrixRow(viewProjection, 0);
			const glm::vec4 row1 = GetMatrixRow(viewProjection, 1);
			const glm::vec4 row2 = GetMatrixRow(viewProjection, 2);
			const glm::vec4 row3 = GetMatrixRow(viewProjection, 3);

			Frustum frustum;
			frustum.Planes[0] = NormalizePlane(row3 + row0);
			frustum.Planes[1] = NormalizePlane(row3 - row0);
			frustum.Planes[2] = NormalizePlane(row3 + row1);
			frustum.Planes[3] = NormalizePlane(row3 - row1);
			frustum.Planes[4] = NormalizePlane(row3 + row2);
			frustum.Planes[5] = NormalizePlane(row3 - row2);
			return frustum;
		}

		bool IsVisible(const AxisAlignedBounds& bounds, const Frustum& frustum)
		{
			if (!bounds.IsValid)
				return true;

			for (const FrustumPlane& plane : frustum.Planes)
			{
				const glm::vec3 positiveVertex =
				{
					plane.Normal.x >= 0.0f ? bounds.Max.x : bounds.Min.x,
					plane.Normal.y >= 0.0f ? bounds.Max.y : bounds.Min.y,
					plane.Normal.z >= 0.0f ? bounds.Max.z : bounds.Min.z
				};

				if (glm::dot(plane.Normal, positiveVertex) + plane.Distance < 0.0f)
					return false;
			}

			return true;
		}

		StaticWorldSurfaceSubmission CreateSubmission(
			const WorldSurface& surface,
			const std::vector<WorldMaterialRef>& materials,
			const Ref<Material>& fallbackMaterial
		)
		{
			StaticWorldSurfaceSubmission submission;
			submission.Material = fallbackMaterial;
			submission.Mesh = surface.MeshAsset;
			submission.Transform = surface.Transform.Matrix;
			submission.SourceSurfaceIndex = surface.SourceSubmeshIndex;

			if (surface.MaterialIndex < materials.size() && materials[surface.MaterialIndex].MaterialAsset)
				submission.Material = materials[surface.MaterialIndex].MaterialAsset;

			return submission;
		}
	}

	void StaticWorldRenderer::Submit(const Ref<StaticWorld>& world, const Ref<Material>& fallbackMaterial, const glm::mat4& viewProjection)
	{
		if (!world || world->IsEmpty())
			return;

		const Frustum frustum = ExtractFrustum(viewProjection);
		const uint32_t totalSurfaces = static_cast<uint32_t>(world->GetSurfaces().size());
		uint32_t visibleSurfaces = 0;
		if (!IsVisible(world->GetWorldBounds(), frustum))
		{
			Renderer::RecordWorldSurfaceStats(totalSurfaces, visibleSurfaces);
			return;
		}

		const std::vector<WorldMaterialRef>& materials = world->GetMaterials();
		for (const WorldSurface& surface : world->GetSurfaces())
		{
			if (!IsVisible(surface.WorldBounds, frustum))
				continue;

			const StaticWorldSurfaceSubmission submission = CreateSubmission(surface, materials, fallbackMaterial);
			Renderer::Submit({ submission.Material, submission.Mesh, submission.Transform });
			visibleSurfaces++;
		}

		Renderer::RecordWorldSurfaceStats(totalSurfaces, visibleSurfaces);
	}
}
