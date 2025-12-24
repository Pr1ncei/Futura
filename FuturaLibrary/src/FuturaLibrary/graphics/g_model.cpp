#include "pch.h"
#include "g_model.h"

Model::Model(Mesh* mesh) : mesh(mesh) {}

void Model::Draw(Shader& shader) const 
{
    shader.setMat4("model", transform.GetMatrix());
    Texture::Use(textures);
    mesh->Draw();
}