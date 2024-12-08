#pragma once
#include <nlohmann/json.hpp>
#include <Scene/scene.h>
namespace fileOperations {

    nlohmann::json serialize(Scene scene);
    
    nlohmann::json serialize(SceneObject obj);

    nlohmann::json serialize(ecs::ECS ecs);

    void saveToFile(const std::string& filepath, const std::string& name, const std::string& ext, Scene scene);
    nlohmann::json loadFromFile(const std::string& filepath);
}