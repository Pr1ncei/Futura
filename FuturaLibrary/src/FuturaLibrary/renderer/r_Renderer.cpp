/**
 *  @file r_Renderer.cpp
 *
 *  @brief Implements the minimal renderer facade used by Phase 2.
 *
 *  The renderer owns frame-level OpenGL state and provides a small submit API
 *  over the lower-level Shader and VertexArray graphics wrappers.
 *
 *      @author:             Prince Pamintuan
 *      @date:               December 24, 2025
 *      Last Modified on:    July 05, 2026
 */

#include "pch.h"
#include "r_Renderer.h"

namespace FuturaLibrary
{
	Renderer::SceneData* Renderer::m_SceneData = nullptr;

	void Renderer::Initialize()
	{
		FT_PROFILE_FUNCTION;
		FT_CORE_ASSERT(!m_SceneData, "Renderer is already initialized!");

		m_SceneData = new SceneData();

		RenderState defaultState;
		RenderCommand::SetDepthTest(defaultState.DepthTest);
		RenderCommand::SetFaceCulling(defaultState.FaceCulling);
		RenderCommand::SetCullFace(defaultState.CullFace);
	}

	void Renderer::BeginFrame(const RenderFrameState& frameState)
	{
		FT_CORE_ASSERT(m_SceneData, "Renderer has not been initialized!");

		m_SceneData->Stats = {};

		RenderCommand::SetDepthTest(frameState.State.DepthTest);
		RenderCommand::SetFaceCulling(frameState.State.FaceCulling);
		if (frameState.State.FaceCulling)
			RenderCommand::SetCullFace(frameState.State.CullFace);
		if (frameState.State.UseViewport)
			RenderCommand::SetViewport(frameState.State.Viewport);

		RenderCommand::Clear(frameState.Clear);
	}

	void Renderer::BeginScene(const glm::mat4& viewProjection)
	{
		FT_CORE_ASSERT(m_SceneData, "Renderer has not been initialized!");
		m_SceneData->ViewProjectionMatrix = viewProjection;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform
	)
	{
		FT_CORE_ASSERT(m_SceneData, "Renderer has not been initialized!");
		FT_CORE_ASSERT(shader, "Renderer::Submit received a null shader!");
		FT_CORE_ASSERT(vertexArray, "Renderer::Submit received a null vertex array!");

		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Model", transform);

		RenderCommand::DrawIndexed(vertexArray);
		m_SceneData->Stats.DrawCalls++;
	}

	void Renderer::Submit(const RenderSubmission& submission)
	{
		FT_CORE_ASSERT(m_SceneData, "Renderer has not been initialized!");
		FT_CORE_ASSERT(submission.Material, "Renderer::Submit received a null material!");
		FT_CORE_ASSERT(submission.Mesh, "Renderer::Submit received a null mesh!");

		m_SceneData->Stats.SubmittedMeshes++;
		m_SceneData->Stats.Triangles += submission.Mesh->GetTriangleCount();
		m_SceneData->Stats.VisibleSurfaces++;

		submission.Material->Bind();
		Submit(submission.Material->GetShader(), submission.Mesh->GetVertexArray(), submission.Transform);
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		Submit({ material, mesh, transform });
	}

	const RenderStats& Renderer::GetStats()
	{
		static RenderStats emptyStats;
		return m_SceneData ? m_SceneData->Stats : emptyStats;
	}
}
