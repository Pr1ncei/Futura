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
 *      Last Modified on:    July 01, 2026
 */

#include "pch.h"
#include "r_Renderer.h"

#include <glad/glad.h>

namespace FuturaLibrary
{
	Renderer::SceneData* Renderer::m_SceneData = nullptr;

	void Renderer::Initialize()
	{
		FT_PROFILE_FUNCTION;
		FT_CORE_ASSERT(!m_SceneData, "Renderer is already initialized!");

		m_SceneData = new SceneData();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		vertexArray->Bind();

		const Ref<IndexBuffer>& indexBuffer = vertexArray->GetIndexBuffer();
		FT_CORE_ASSERT(indexBuffer, "Renderer::Submit currently requires an index buffer!");
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
