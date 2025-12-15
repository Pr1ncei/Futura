#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement 
{
    FORWARD,
    BACKWARD, 
    LEFT, 
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f; 
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f; 

class Camera 
{
private:    
    void updateCameraVectors();

public:
    // Camera Attributes
    glm::vec3 Position; 
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;  

    // Euler 
    GLfloat Yaw; 
    GLfloat Pitch; 
    // glm::vec3 Roll;

    GLfloat MovementSpeed; 
    GLfloat MouseSensitivity; 
    GLfloat Zoom;

    Camera(
        glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat yaw = YAW, 
        GLfloat pitch = PITCH
    ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(
        GLfloat posX, 
        GLfloat posY,
        GLfloat posZ,
        GLfloat upX,
        GLfloat upY,
        GLfloat upZ,
        GLfloat yaw,
        GLfloat pitch
    ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {   
        Position = glm::vec3(posX, posY, posZ); 
        WorldUp = glm::vec3(upX, upY, upZ); 
        Yaw = yaw; 
        Pitch = pitch; 
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
};


#endif