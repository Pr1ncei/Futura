#include "pch.h"
#include "g_model_manager.h"
#include "g_model.h"
#include "../utils/u_log.h"

std::unordered_map<std::string, std::shared_ptr<Model>> ModelManager::s_models;


std::shared_ptr<Model> ModelManager::Load(const std::string &path)
{
    auto it = s_models.find(path);
    if (it != s_models.end())
        return it->second;


    // havent added assimp yet
    //std::shared_ptr<Model> model = std::make_shared<Model>(LoadAssimpFromFile(path));

    // s_models[path] = model; 
    // return model;
}

std::shared_ptr<Model> ModelManager::Get(const std::string &path)
{
    return s_models[path];
}

void ModelManager::Clear()
{   
    s_models.clear();
}