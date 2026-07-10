/**
 *  @file r_DebugRenderer.cpp
 *
 *  @brief Implements line-based debug drawing used for renderer and world inspection.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 05, 2026
 *      Last Modified on:    July 05, 2026
 */

#include "pch.h"
#include "r_DebugRenderer.h"

#include "FuturaLibrary/graphics/g_Buffer.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"
#include "FuturaLibrary/renderer/r_RenderCommand.h"
#include "FuturaLibrary/resources/r_StaticWorld.h"

namespace FuturaLibrary
{
	namespace
	{
		struct DebugLineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct DebugRendererData
		{
			Ref<Shader> ShaderAsset;
			Ref<VertexArray> VertexArrayAsset;
			Ref<VertexBuffer> VertexBufferAsset;
			std::vector<DebugLineVertex> Vertices;
			glm::mat4 ViewProjection = glm::mat4(1.0f);
			DebugDrawStats Stats;
			uint32_t MaxVertices = 0;
			bool SceneActive = false;
		};

		DebugRendererData* s_Data = nullptr;

	}

	void DebugRenderer::Initialize(const Ref<Shader>& shader, uint32_t maxLines)
	{
		FT_CORE_ASSERT(!s_Data, "DebugRenderer is already initialized!");
		FT_CORE_ASSERT(shader, "DebugRenderer requires a shader!");
		FT_CORE_ASSERT(maxLines > 0, "DebugRenderer requires at least one line!");

		s_Data = new DebugRendererData();
		s_Data->ShaderAsset = shader;
		s_Data->MaxVertices = maxLines * 2;
		s_Data->Vertices.reserve(s_Data->MaxVertices);

		s_Data->VertexArrayAsset = CreateRef<VertexArray>();
		s_Data->VertexBufferAsset = CreateRef<VertexBuffer>(nullptr, s_Data->MaxVertices * sizeof(DebugLineVertex));
		s_Data->VertexBufferAsset->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		});
		s_Data->VertexArrayAsset->AddVertexBuffer(s_Data->VertexBufferAsset);
	}

	void DebugRenderer::Shutdown()
	{
		delete s_Data;
		s_Data = nullptr;
	}

	void DebugRenderer::BeginScene(const glm::mat4& viewProjection)
	{
		FT_CORE_ASSERT(s_Data, "DebugRenderer has not been initialized!");
		s_Data->ViewProjection = viewProjection;
		s_Data->Vertices.clear();
		s_Data->Stats = {};
		s_Data->SceneActive = true;
	}

	void DebugRenderer::EndScene()
	{
		FT_CORE_ASSERT(s_Data, "DebugRenderer has not been initialized!");
		Flush();
		s_Data->SceneActive = false;
	}

	void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
	{
		FT_CORE_ASSERT(s_Data, "DebugRenderer has not been initialized!");
		FT_CORE_ASSERT(s_Data->SceneActive, "DebugRenderer::DrawLine must be called between BeginScene and EndScene!");

		if (s_Data->Vertices.size() + 2 > s_Data->MaxVertices)
			Flush();

		s_Data->Vertices.push_back({ start, color });
		s_Data->Vertices.push_back({ end, color });
		s_Data->Stats.LineCount++;
		s_Data->Stats.VertexCount += 2;
	}

	void DebugRenderer::DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color)
	{
		const glm::vec3 corners[8] =
		{
			{ min.x, min.y, min.z },
			{ max.x, min.y, min.z },
			{ max.x, max.y, min.z },
			{ min.x, max.y, min.z },
			{ min.x, min.y, max.z },
			{ max.x, min.y, max.z },
			{ max.x, max.y, max.z },
			{ min.x, max.y, max.z }
		};

		const uint32_t edges[24] =
		{
			0, 1, 1, 2, 2, 3, 3, 0,
			4, 5, 5, 6, 6, 7, 7, 4,
			0, 4, 1, 5, 2, 6, 3, 7
		};

		for (uint32_t i = 0; i < 24; i += 2)
			DrawLine(corners[edges[i]], corners[edges[i + 1]], color);
	}

	void DebugRenderer::DrawBox(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color)
	{
		DrawAABB(center - halfExtents, center + halfExtents, color);
	}

	void DebugRenderer::DrawStaticWorld(const StaticWorld& world, const DebugWorldDrawSettings& settings)
	{
		if (settings.DrawBounds)
		{
			const AxisAlignedBounds& worldBounds = world.GetWorldBounds();
			if (worldBounds.IsValid)
				DrawAABB(worldBounds.Min, worldBounds.Max, glm::vec4(0.1f, 0.75f, 1.0f, 1.0f));

			const std::vector<WorldSurface>& surfaces = world.GetSurfaces();
			for (uint32_t i = 0; i < surfaces.size(); i++)
			{
				const WorldSurface& surface = surfaces[i];
				if (!surface.WorldBounds.IsValid)
					continue;

				DrawAABB(surface.WorldBounds.Min, surface.WorldBounds.Max, glm::vec4(0.25f, 1.0f, 0.35f, 0.85f));
			}
		}
	}

	const DebugDrawStats& DebugRenderer::GetStats()
	{
		static DebugDrawStats emptyStats;
		return s_Data ? s_Data->Stats : emptyStats;
	}

	bool DebugRenderer::IsInitialized()
	{
		return s_Data != nullptr;
	}

	void DebugRenderer::Flush()
	{
		if (!s_Data || s_Data->Vertices.empty())
			return;

		s_Data->ShaderAsset->Bind();
		s_Data->ShaderAsset->SetMat4("u_ViewProjection", s_Data->ViewProjection);
		s_Data->VertexBufferAsset->SetData(
			s_Data->Vertices.data(),
			static_cast<uint32_t>(s_Data->Vertices.size() * sizeof(DebugLineVertex))
		);

		RenderCommand::SetLineWidth(1.5f);
		RenderCommand::DrawLines(s_Data->VertexArrayAsset, static_cast<uint32_t>(s_Data->Vertices.size()));
		s_Data->Vertices.clear();
		s_Data->Stats.FlushCount++;
	}
}
