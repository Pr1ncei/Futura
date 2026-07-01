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
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	constexpr int KeyW = 87;
	constexpr int KeyA = 65;
	constexpr int KeyS = 83;
	constexpr int KeyD = 68;

	void ProcessCameraKeyboard(
		Camera& camera,
		bool moveForward,
		bool moveBackward,
		bool moveLeft,
		bool moveRight,
		float deltaTime
	)
	{
		const float velocity = camera.MovementSpeed * deltaTime;

		if (moveForward)
			camera.Position += camera.Front * velocity;
		if (moveBackward)
			camera.Position -= camera.Front * velocity;
		if (moveLeft)
			camera.Position -= camera.Right * velocity;
		if (moveRight)
			camera.Position += camera.Right * velocity;
	}
}

GameLayer::GameLayer()
	: FuturaLibrary::Layer("GameLayer"), m_Camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
}

void GameLayer::OnAttach()
{
	FuturaLibrary::Renderer::Initialize();

	auto shader = FuturaLibrary::ResourceManager::LoadShader("RendererTest", "shaders/RendererTest.glsl");
	auto texture = FuturaLibrary::ResourceManager::LoadTexture2D("Gate", "texture/gate.jpg");

	m_CubeMesh = FuturaLibrary::Mesh::CreateCube();
	m_CubeMaterial = FuturaLibrary::CreateRef<FuturaLibrary::Material>(shader, texture);

	FuturaLibrary::Application::Get().GetWindow().SetCursorVisibility();
	m_LastFrameTime = static_cast<float>(FuturaLibrary::Application::Get().GetWindow().GetTime());
}

void GameLayer::OnUpdate()
{
	FuturaLibrary::Window& window = FuturaLibrary::Application::Get().GetWindow();
	const float currentTime = static_cast<float>(window.GetTime());
	const float deltaTime = currentTime - m_LastFrameTime;
	m_LastFrameTime = currentTime;

	ProcessCameraKeyboard(
		m_Camera,
		m_MoveForward,
		m_MoveBackward,
		m_MoveLeft,
		m_MoveRight,
		deltaTime
	);
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

	FuturaLibrary::Renderer::BeginScene(viewProjection);
	FuturaLibrary::Renderer::Submit(m_CubeMaterial, m_CubeMesh, transform);
	FuturaLibrary::Renderer::EndScene();
}

void GameLayer::OnEvent(FuturaLibrary::Event& event)
{
	FuturaLibrary::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<FuturaLibrary::KeyPressedEvent>(
		std::bind(&GameLayer::OnKeyPressed, this, std::placeholders::_1)
	);
	dispatcher.Dispatch<FuturaLibrary::KeyReleasedEvent>(
		std::bind(&GameLayer::OnKeyReleased, this, std::placeholders::_1)
	);
	dispatcher.Dispatch<FuturaLibrary::MouseMovedEvent>(
		std::bind(&GameLayer::OnMouseMoved, this, std::placeholders::_1)
	);
	dispatcher.Dispatch<FuturaLibrary::MouseScrolledEvent>(
		std::bind(&GameLayer::OnMouseScrolled, this, std::placeholders::_1)
	);
}

bool GameLayer::OnKeyPressed(FuturaLibrary::KeyPressedEvent& event)
{
	switch (event.GetKeyCode())
	{
		case KeyW: m_MoveForward = true; break;
		case KeyS: m_MoveBackward = true; break;
		case KeyA: m_MoveLeft = true; break;
		case KeyD: m_MoveRight = true; break;
		default: break;
	}

	return false;
}

bool GameLayer::OnKeyReleased(FuturaLibrary::KeyReleasedEvent& event)
{
	switch (event.GetKeyCode())
	{
		case KeyW: m_MoveForward = false; break;
		case KeyS: m_MoveBackward = false; break;
		case KeyA: m_MoveLeft = false; break;
		case KeyD: m_MoveRight = false; break;
		default: break;
	}

	return false;
}

bool GameLayer::OnMouseMoved(FuturaLibrary::MouseMovedEvent& event)
{
	const float mouseX = event.GetX();
	const float mouseY = event.GetY();

	if (m_FirstMouseMove)
	{
		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;
		m_FirstMouseMove = false;
		return false;
	}

	const float xOffset = mouseX - m_LastMouseX;
	const float yOffset = m_LastMouseY - mouseY;
	m_LastMouseX = mouseX;
	m_LastMouseY = mouseY;

	m_Camera.ProcessMouseMovement(xOffset, yOffset);
	return false;
}

bool GameLayer::OnMouseScrolled(FuturaLibrary::MouseScrolledEvent& event)
{
	m_Camera.Zoom -= event.GetYOffset();
	m_Camera.Zoom = std::clamp(m_Camera.Zoom, 1.0f, 45.0f);
	return false;
}
