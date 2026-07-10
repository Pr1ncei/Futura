#ifndef RENDERER_H
#define RENDERER_H 
#include "pch.h"

#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"
#include "FuturaLibrary/renderer/r_Material.h"
#include "FuturaLibrary/renderer/r_Mesh.h"
#include "FuturaLibrary/renderer/r_RenderCommand.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	struct RenderFrameState
	{
		RenderClearState Clear;
		RenderState State;
	};

	struct RenderSubmission
	{
		Ref<Material> Material;
		Ref<Mesh> Mesh;
		glm::mat4 Transform = glm::mat4(1.0f);
	};

	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t SubmittedMeshes = 0;
		uint32_t Triangles = 0;
		uint32_t VisibleSurfaces = 0;
	};

	class FT_API Renderer
	{
	public: 
		static void Initialize(); 
		static void BeginFrame(const RenderFrameState& frameState);
		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene(); 
		static void Submit(const RenderSubmission& submission);
		static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform);
		static const RenderStats& GetStats();

	private: 
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
			RenderStats Stats;
		};

		static SceneData* m_SceneData; 

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform); 


		class WorldRenderer; 
		class BrushRenderer; 
		class SpriteRenderer; 
	};
}

#endif
