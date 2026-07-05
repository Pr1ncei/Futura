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
#include "FuturaLibrary/renderer/r_DebugRenderer.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include "FuturaLibrary/renderer/r_RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

GameLayer::GameLayer()
	: FuturaLibrary::Layer("GameLayer"), m_CameraController(glm::vec3(0.0f, 8.0f, 95.0f))
{
}

void GameLayer::OnAttach()
{
	FuturaLibrary::Renderer::Initialize();

	auto shader = FuturaLibrary::ResourceManager::LoadShader("RendererTest", "shaders/RendererTest.glsl");
	auto debugShader = FuturaLibrary::ResourceManager::LoadShader("DebugLine", "shaders/DebugLine.glsl");

	m_DefaultMaterial = FuturaLibrary::CreateRef<FuturaLibrary::Material>(shader);
	m_TestCityModel = FuturaLibrary::ResourceManager::LoadModel("TestCity", "models/Test.obj", shader);
	FuturaLibrary::DebugRenderer::Initialize(debugShader);

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

	m_FPSUpdateTimer += deltaTime;
	m_FrameCounter++;
	if (m_FPSUpdateTimer >= 0.5f)
	{
		const float fps = static_cast<float>(m_FrameCounter) / m_FPSUpdateTimer;
		const float frameMs = fps > 0.0f ? 1000.0f / fps : 0.0f;

		std::ostringstream title;
		title << "Futura | FPS: " << static_cast<int>(fps)
			<< " | Frame: " << frameMs << " ms"
			<< " | Debug Lines: " << FuturaLibrary::DebugRenderer::GetStats().LineCount;
		window.SetTitle(title.str());

		m_FPSUpdateTimer = 0.0f;
		m_FrameCounter = 0;
	}
}

void GameLayer::OnRender()
{
	FuturaLibrary::RenderFrameState frameState;
	frameState.Clear.Color = glm::vec4(0.08f, 0.09f, 0.08f, 1.0f);
	FuturaLibrary::Renderer::BeginFrame(frameState);

	FuturaLibrary::Window& window = FuturaLibrary::Application::Get().GetWindow();
	glm::mat4 viewProjection = m_CameraController.GetCamera().GetViewProjectionMatrix(window.GetAspectRatio());

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(0.001f));
	transform = glm::translate(transform, glm::vec3(-216.9258f, -3469.41f, -13499.998f));

	FuturaLibrary::Renderer::BeginScene(viewProjection);
	if (m_TestCityModel && !m_TestCityModel->IsEmpty())
	{
		for (const FuturaLibrary::ModelSubmesh& submesh : m_TestCityModel->GetSubmeshes())
			FuturaLibrary::Renderer::Submit({ submesh.MaterialAsset ? submesh.MaterialAsset : m_DefaultMaterial, submesh.MeshAsset, transform });
	}
	FuturaLibrary::Renderer::EndScene();

	FuturaLibrary::DebugRenderer::BeginScene(viewProjection);
	FuturaLibrary::DebugRenderer::EndScene();
	FuturaLibrary::RenderCommand::CheckErrors("GameLayer::OnRender");
}

void GameLayer::OnEvent(FuturaLibrary::Event& event)
{
	m_CameraController.OnEvent(event);
}
