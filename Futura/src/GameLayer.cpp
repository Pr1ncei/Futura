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
 *      Last Modified on:    July 05, 2026
 */

#include "GameLayer.h"

#include "FuturaLibrary/core/c_application.h"
#include "FuturaLibrary/resources/r_ResourceManager.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer()
	: FuturaLibrary::Layer("GameLayer"), m_CameraController(glm::vec3(0.0f, 0.0f, 3.0f))
{
}

void GameLayer::OnAttach()
{
	FuturaLibrary::Renderer::Initialize();

	auto shader = FuturaLibrary::ResourceManager::LoadShader("RendererTest", "shaders/RendererTest.glsl");
	auto texture = FuturaLibrary::ResourceManager::LoadTexture2D("Gate", "texture/gate.jpg");

	m_CubeMesh = FuturaLibrary::Mesh::CreateCube();
	m_CubeMaterial = FuturaLibrary::CreateRef<FuturaLibrary::Material>(shader);
	m_CubeMaterial->SetAlbedoTexture(texture);
	m_RubixCubeModel = FuturaLibrary::ResourceManager::LoadModel("RubixCube", "models/RubixCube.obj", shader);

	FuturaLibrary::Application::Get().GetWindow().SetCursorVisibility();
	m_LastFrameTime = static_cast<float>(FuturaLibrary::Application::Get().GetWindow().GetTime());
}

void GameLayer::OnUpdate()
{
	FuturaLibrary::Window& window = FuturaLibrary::Application::Get().GetWindow();
	const float currentTime = static_cast<float>(window.GetTime());
	const float deltaTime = currentTime - m_LastFrameTime;
	m_LastFrameTime = currentTime;

	m_CameraController.OnUpdate(deltaTime);
}

void GameLayer::OnRender()
{
	FuturaLibrary::RenderFrameState frameState;
	frameState.Clear.Color = glm::vec4(0.08f, 0.09f, 0.08f, 1.0f);
	FuturaLibrary::Renderer::BeginFrame(frameState);

	FuturaLibrary::Window& window = FuturaLibrary::Application::Get().GetWindow();
	glm::mat4 viewProjection = m_CameraController.GetCamera().GetViewProjectionMatrix(window.GetAspectRatio());

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, static_cast<float>(window.GetTime()), glm::vec3(0.25f, 1.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(0.75f));

	FuturaLibrary::Renderer::BeginScene(viewProjection);
	if (m_RubixCubeModel && !m_RubixCubeModel->IsEmpty())
	{
		for (const FuturaLibrary::ModelSubmesh& submesh : m_RubixCubeModel->GetSubmeshes())
		{
			if (submesh.Name.find("Plane") != std::string::npos)
				continue;

			FuturaLibrary::Renderer::Submit({ submesh.MaterialAsset ? submesh.MaterialAsset : m_CubeMaterial, submesh.MeshAsset, transform });
		}
	}
	else
	{
		FuturaLibrary::Renderer::Submit({ m_CubeMaterial, m_CubeMesh, transform });
	}
	FuturaLibrary::Renderer::EndScene();
}

void GameLayer::OnEvent(FuturaLibrary::Event& event)
{
	m_CameraController.OnEvent(event);
}
