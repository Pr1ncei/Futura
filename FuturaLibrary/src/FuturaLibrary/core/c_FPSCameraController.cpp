#include "pch.h"
#include "c_FPSCameraController.h"

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

		if (m_MoveForward)
			m_Camera.Move(m_Camera.GetFront() * velocity);

		if (m_MoveBackward)
			m_Camera.Move(-m_Camera.GetFront() * velocity);

		if (m_MoveLeft)
			m_Camera.Move(-m_Camera.GetRight() * velocity);

		if (m_MoveRight)
			m_Camera.Move(m_Camera.GetRight() * velocity);
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
