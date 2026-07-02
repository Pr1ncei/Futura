#pragma once

#include "FuturaLibrary/core/c_PerspectiveCamera.h"
#include "FuturaLibrary/events/e_Event.h"
#include "FuturaLibrary/events/e_KeyEvent.h"
#include "FuturaLibrary/events/e_MouseEvent.h"

namespace FuturaLibrary
{
	class FT_API FPSCameraController
	{
	public:
		FPSCameraController(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f));

		void OnUpdate(float deltaTime);
		void OnEvent(Event& event);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);

	private:
		PerspectiveCamera m_Camera;

		float m_MovementSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;

		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;
		bool m_FirstMouseMove = true;

		bool m_MoveForward = false;
		bool m_MoveBackward = false;
		bool m_MoveLeft = false;
		bool m_MoveRight = false;
	};
}
