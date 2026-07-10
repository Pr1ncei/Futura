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
 *      Last Modified on:    July 10, 2026
 */

#include "GameLayer.h"

#include "FuturaLibrary/core/c_application.h"
#include "FuturaLibrary/resources/r_ResourceManager.h"
#include "FuturaLibrary/renderer/r_DebugOverlay.h"
#include "FuturaLibrary/renderer/r_DebugRenderer.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include "FuturaLibrary/renderer/r_RenderCommand.h"
#include <glm/glm.hpp>

namespace
{
	constexpr int KeyF1 = 290; // Toggle world and surface bounds.
	constexpr int KeyF2 = 291; // Toggle world plane normals when plane data exists.
	constexpr int KeyF3 = 292; // Toggle camera frustum outline.
	constexpr int KeyF5 = 294; // Select the next surface for bounds highlighting.
	constexpr int KeyF6 = 295; // Toggle the in-game debug statistics overlay.
}

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
	m_SceneWorld.LoadPreviewScene("scenes/city_preview.scene", shader);
	FuturaLibrary::DebugRenderer::Initialize(debugShader);
	m_CameraController.SetMovementResolver([this](const glm::vec3& cameraPosition, const glm::vec3& desiredDelta)
	{
		return m_SceneWorld.ResolveCameraMovement(cameraPosition, desiredDelta);
	});

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

		m_DebugOverlayFrameData.FPS = fps;
		m_DebugOverlayFrameData.FrameTimeMs = frameMs;
		window.SetTitle("Futura");

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

	FuturaLibrary::Renderer::BeginScene(viewProjection);
	m_SceneWorld.Submit(m_DefaultMaterial);
	FuturaLibrary::Renderer::EndScene();

	FuturaLibrary::DebugRenderer::BeginScene(viewProjection);
	m_DebugOverlayState.DrawSettings.InverseViewProjection = glm::inverse(viewProjection);
	m_SceneWorld.DrawDebug(m_DebugOverlayState.DrawSettings);
	FuturaLibrary::DebugRenderer::EndScene();
	FuturaLibrary::RenderCommand::CheckErrors("GameLayer::OnRender");
}

void GameLayer::OnImGuiRender()
{
	m_DebugOverlayFrameData.Render = FuturaLibrary::Renderer::GetStats();
	m_DebugOverlayFrameData.DebugDraw = FuturaLibrary::DebugRenderer::GetStats();
	m_DebugOverlayFrameData.Collision = m_SceneWorld.GetCollisionStats();
	m_DebugOverlayFrameData.HasPlanes = m_SceneWorld.HasPlanes();
	m_DebugOverlayFrameData.SelectedSurfaceIndex = m_SceneWorld.GetSelectedSurfaceIndex();

	FuturaLibrary::DebugOverlay::Draw(m_DebugOverlayState, m_DebugOverlayFrameData, [this]()
	{
		m_SceneWorld.SelectNextSurface();
	});
}

void GameLayer::OnEvent(FuturaLibrary::Event& event)
{
	FuturaLibrary::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<FuturaLibrary::KeyPressedEvent>(
		std::bind(&GameLayer::OnKeyPressed, this, std::placeholders::_1)
	);

	m_CameraController.OnEvent(event);
}

bool GameLayer::OnKeyPressed(FuturaLibrary::KeyPressedEvent& event)
{
	if (event.GetRepeatCount() > 0)
		return false;

	switch (event.GetKeyCode())
	{
		case KeyF1:
			m_DebugOverlayState.DrawSettings.DrawBounds = !m_DebugOverlayState.DrawSettings.DrawBounds;
			return false;
		case KeyF2:
			if (!m_SceneWorld.HasPlanes())
			{
				FT_CORE_WARN("No world plane data is available yet. Plane debug will become visible after plane extraction is implemented.");
				return false;
			}

			m_DebugOverlayState.DrawSettings.DrawPlanes = !m_DebugOverlayState.DrawSettings.DrawPlanes;
			return false;
		case KeyF3:
			m_DebugOverlayState.DrawSettings.DrawFrustum = !m_DebugOverlayState.DrawSettings.DrawFrustum;
			return false;
		case KeyF5:
			m_SceneWorld.SelectNextSurface();
			if (!m_DebugOverlayState.DrawSettings.DrawBounds)
				FT_CORE_INFO("Selected surface changed. Enable F1 bounds to see the highlighted surface.");
			return false;
		case KeyF6:
			m_DebugOverlayState.ShowStats = !m_DebugOverlayState.ShowStats;
			return false;
		default:
			return false;
	}
}
