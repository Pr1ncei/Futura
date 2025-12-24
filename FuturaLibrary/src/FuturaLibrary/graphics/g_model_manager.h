// Initial Model Manager that manages all the models

#ifndef MODEL_MANAGER_h
#define MODEL_MANAGER_H

#include "pch.h"

class Model;

class ModelManager
{
public:
    static std::shared_ptr<Model> Load(const std::string &path); 
    static std::shared_ptr<Model> Get(const std::string &path); 
    static void Clear();
private:    
    static std::unordered_map<std::string, std::shared_ptr<Model>> s_models;
};

#endif