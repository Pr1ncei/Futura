#include "pch.h"
#include "c_FPSCameraController.h"

#include <glm/gtx/norm.hpp>

namespace FuturaLibrary
{
	namespace
	{
		constexpr int KeyW = 87;
		constexpr int KeyA = 65;
		constexpr int KeyS = 83;
		constexpr int KeyD = 68;
	}

	FPSCameraController::FPSCameraController(const glm::vec3& position)
		: m_Camera(position)
	{
	}

	void FPSCameraController::OnUpdate(float deltaTime)
	{
		const float velocity = m_MovementSpeed * deltaTime;
		glm::vec3 desiredDelta = glm::vec3(0.0f);

		if (m_MoveForward)
			desiredDelta += m_Camera.GetFront() * velocity;

		if (m_MoveBackward)
			desiredDelta += -m_Camera.GetFront() * velocity;

		if (m_MoveLeft)
			desiredDelta += -m_Camera.GetRight() * velocity;

		if (m_MoveRight)
			desiredDelta += m_Camera.GetRight() * velocity;

		if (glm::length2(desiredDelta) <= 0.0f)
			return;

		const glm::vec3 resolvedDelta = m_MovementResolver ? m_MovementResolver(m_Camera.GetPosition(), desiredDelta) : desiredDelta;
		m_Camera.Move(resolvedDelta);
	}

	void FPSCameraController::SetMovementResolver(const std::function<glm::vec3(const glm::vec3&, const glm::vec3&)>& resolver)
	{
		m_MovementResolver = resolver;
	}

	void FPSCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyPressedEvent>(
			std::bind(&FPSCameraController::OnKeyPressed, this, std::placeholders::_1)
		);

		dispatcher.Dispatch<KeyReleasedEvent>(
			std::bind(&FPSCameraController::OnKeyReleased, this, std::placeholders::_1)
		);

		dispatcher.Dispatch<MouseMovedEvent>(
			std::bind(&FPSCameraController::OnMouseMoved, this, std::placeholders::_1)
		);

		dispatcher.Dispatch<MouseScrolledEvent>(
			std::bind(&FPSCameraController::OnMouseScrolled, this, std::placeholders::_1)
		);
	}

	bool FPSCameraController::OnKeyPressed(KeyPressedEvent& event)
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

	bool FPSCameraController::OnKeyReleased(KeyReleasedEvent& event)
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

	bool FPSCameraController::OnMouseMoved(MouseMovedEvent& event)
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

		m_Camera.Rotate(
			xOffset * m_MouseSensitivity,
			yOffset * m_MouseSensitivity
		);

		return false;
	}

	bool FPSCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_Camera.Zoom(event.GetYOffset());
		return false;
	}
}
