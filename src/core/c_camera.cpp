/**
 *  @file input.cpp
 * 
 *  @brief Implements the Camera Class 
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 13, 2025 (12:17PM)
 *      Last Modified on:       December 15, 2025 (11:11AM)
 */


#include "c_camera.h"

void Camera::updateCameraVectors()
{
    glm::vec3 front; 
    
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMouseMovement(
    GLfloat xoffset,
    GLfloat yoffset,
    GLboolean constrainPitch
)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors(); 
}