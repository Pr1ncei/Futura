/**
 *  @file GameLayer.cpp
 *
 *  @brief Implements the first renderer-abstraction test layer.
 *
 *  This layer intentionally renders one textured cube. The goal is not
 *  gameplay yet; it is to prove Phase 2's application loop, graphics wrappers,
 *  texture loading, and renderer facade work together.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 01, 2026
 *      Last Modified on:    July 01, 2026
 */

#include "GameLayer.h"

#include "FuturaLibrary/core/c_application.h"
#include "FuturaLibrary/resources/r_ResourceManager.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	float s_CubeVertices[] =
	{
		// front
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		// back
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		// left
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		// right
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		// top
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		// bottom
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f
	};

	uint32_t s_CubeIndices[] =
	{
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
}

GameLayer::GameLayer()
	: FuturaLibrary::Layer("GameLayer"), m_Camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
}

void GameLayer::OnAttach()
{
	FuturaLibrary::Renderer::Initialize();

	m_Shader = FuturaLibrary::ResourceManager::LoadShader("RendererTest", "shaders/RendererTest.glsl");
	m_Texture = FuturaLibrary::ResourceManager::LoadTexture2D("Gate", "texture/gate.jpg");

	m_VertexArray = FuturaLibrary::CreateRef<FuturaLibrary::VertexArray>();

	FuturaLibrary::Ref<FuturaLibrary::VertexBuffer> vertexBuffer =
		FuturaLibrary::CreateRef<FuturaLibrary::VertexBuffer>(s_CubeVertices, sizeof(s_CubeVertices));
	vertexBuffer->SetLayout({
		{ FuturaLibrary::ShaderDataType::Float3, "a_Position" },
		{ FuturaLibrary::ShaderDataType::Float2, "a_TexCoord" }
	});
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	FuturaLibrary::Ref<FuturaLibrary::IndexBuffer> indexBuffer =
		FuturaLibrary::CreateRef<FuturaLibrary::IndexBuffer>(
			s_CubeIndices,
			static_cast<uint32_t>(sizeof(s_CubeIndices) / sizeof(uint32_t))
		);
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_Shader->Bind();
	m_Shader->SetInt("u_Texture", 0);
}

void GameLayer::OnUpdate()
{
}

void GameLayer::OnRender()
{
	FuturaLibrary::Renderer::SetClearColor(glm::vec4(0.08f, 0.09f, 0.08f, 1.0f));
	FuturaLibrary::Renderer::Clear();

	FuturaLibrary::Window& window = FuturaLibrary::Application::Get().GetWindow();
	glm::mat4 projection = glm::perspective(
		glm::radians(m_Camera.Zoom),
		window.GetAspectRatio(),
		0.1f,
		100.0f
	);
	glm::mat4 viewProjection = projection * m_Camera.GetViewMatrix();

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, static_cast<float>(window.GetTime()), glm::vec3(0.25f, 1.0f, 0.0f));

	m_Texture->Bind(0);
	FuturaLibrary::Renderer::BeginScene(viewProjection);
	FuturaLibrary::Renderer::Submit(m_Shader, m_VertexArray, transform);
	FuturaLibrary::Renderer::EndScene();
}
