#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "pch.h"
#include "../core/c_core.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Models
{
    extern FT_API std::vector<GLfloat> vertices;
    extern FT_API glm::vec3 cubePositions[];

    void Transform();
}

#endif