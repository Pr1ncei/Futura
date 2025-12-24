/**
 *  @file g_model.h
 * 
 *  @brief Model clsas represents as one renderable object in the scene.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 16, 2025 (6:33PM)
 */

#ifndef MODEL_H
#define MODEL_H

#include "pch.h"
#include "g_mesh.h"
#include "g_texture.h"
#include "g_transform.h"
#include "g_shader.h"

class Model
{
public: 
    Model(Mesh* mesh);
    void Draw(Shader& shader) const;
    Transform transform;    
    std::vector<GLuint> textures;
private:
    Mesh* mesh; 
};

#endif