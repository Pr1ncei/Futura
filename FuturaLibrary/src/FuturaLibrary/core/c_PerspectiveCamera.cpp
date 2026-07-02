#include "pch.h"
#include "c_PerspectiveCamera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace FuturaLibrary
{
	PerspectiveCamera::PerspectiveCamera(
		const glm::vec3& position,
		float yaw,
		float pitch,
		float fov
	) : m_Position(position), m_Yaw(yaw), m_Pitch(pitch), m_FOV(fov)
	{
		RecalculateVectors();
	}

	void PerspectiveCamera::Move(const glm::vec3& delta)
	{
		m_Position += delta;
	}

	void PerspectiveCamera::Rotate(float yawOffset, float pitchOffset)
	{
		m_Yaw += yawOffset;
		m_Pitch += pitchOffset; 
		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		RecalculateVectors();
	}

	void PerspectiveCamera::Zoom(float offset)
	{
		m_FOV -= offset; 
		m_FOV = std::clamp(m_FOV, 1.0f, 45.0f);
	}

	glm::mat4 PerspectiveCamera::GetViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	glm::mat4 PerspectiveCamera::GetProjectionMatrix(float aspectRatio) const
	{
		return glm::perspective(glm::radians(m_FOV), aspectRatio, 0.1f, 100.0f);
	}

	glm::mat4 PerspectiveCamera::GetViewProjectionMatrix(float aspectRatio) const
	{
		return GetProjectionMatrix(aspectRatio) * GetViewMatrix();
	}

	void PerspectiveCamera::RecalculateVectors()
	{
		glm::vec3 front; 
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Front = glm::normalize(front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
}
