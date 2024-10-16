#include "Scene/ECS/ecs.h"

ecs::ECS::ECS() {
}

ecs::Entity ecs::ECS::createEntity() {
    Entity entity = nextEntityId++;
    entityMasks[entity].reset();
    return entity;
}

const std::unordered_map<ecs::Entity, std::bitset<MAX_COMPONENTS>>& ecs::ECS::getEntityMasks()const {
    return entityMasks;
}

void ecs::ECS::updateSystems() {
    for (auto& system : systems) {
        system->update(*this); // Wywo�anie metody update
    }
}

void ecs::ECS::addSystem(std::shared_ptr<ECSSystem> system) {
     systems.push_back(system);
}

std::vector<std::shared_ptr<Component>> ecs::ECS::getAllComponents(Entity entity) const{
    std::vector<std::shared_ptr<Component>> result;

    for (const auto& [typeIndex, componentMap] : components) {
        if (componentMap.find(entity) != componentMap.end()) {
            result.push_back(std::static_pointer_cast<Component>(componentMap.at(entity)));
        }
    }

    return result;
}
