#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Models
{
    extern std::vector<GLfloat> vertices;
    extern glm::vec3 cubePositions[];

    void Transform();

}


#endif