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
        system->update(*this); // Wywo³anie metody update
    }
}

void ecs::ECS::addSystem(std::shared_ptr<ECSSystem> system) {
     systems.push_back(system);
}
