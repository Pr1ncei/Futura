#pragma once

#include <glm/glm.hpp>
#include "FuturaLibrary/core/c_core.h"

namespace FuturaLibrary
{
	class FT_API PerspectiveCamera
	{
	public: 
		PerspectiveCamera(
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
			float yaw = -90.0f,
			float pitch = 0.0f,
			float fov = 45.0f
		);

		const glm::vec3& GetPosition() const    { return m_Position; }
        const glm::vec3& GetFront() const       { return m_Front; }
        const glm::vec3& GetRight() const       { return m_Right; }
        float GetFOV() const                    { return m_FOV; }

        void Move(const glm::vec3& delta);
        void Rotate(float yawOffset, float pitchOffset);
        void Zoom(float offset); 

        glm::mat4 GetViewMatrix() const; 
        glm::mat4 GetProjectionMatrix(float aspectRatio) const; 
        glm::mat4 GetViewProjectionMatrix(float aspectRatio) const; 

	private: 
		void RecalculateVectors();

	private: 
		glm::vec3 m_Position; 
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float m_Yaw; 
		float m_Pitch; 
		float m_FOV; 
	};
}

