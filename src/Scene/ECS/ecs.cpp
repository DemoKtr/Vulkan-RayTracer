#include "Scene/ECS/ecs.h"
#include <iostream>

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

std::vector<std::shared_ptr<Component>> ecs::ECS::getAllComponents(Entity entity) const {
    std::vector<std::shared_ptr<Component>> result;

    for (const auto& [typeIndex, componentMap] : components) {
        auto it = componentMap.find(entity);
        if (it != componentMap.end()) {
            // Iterujemy przez wszystkie komponenty w wektorze
            for (const auto& component : it->second) {
                result.push_back(component);
            }
        }
    }

    return result;
}

void ecs::ECS::removeComponent(Component* component, Entity entity) {

    std::type_index typeIndex = std::type_index(typeid(*component));


    auto typeIt = components.find(typeIndex);
    if (typeIt != components.end()) {
        auto& entityMap = typeIt->second;
       // std::cout << entityMap.size() << std::endl;
        printNumberOfComponents(entity);
        auto entityIt = entityMap.find(entity);
        if (entityIt != entityMap.end()) {

            entityMap.erase(entity);
            printNumberOfComponents(entity);
           // std::cout << entityMap.size() << std::endl;
            if (entityMap.empty()) {
                components.erase(typeIt);
            }
            
        }
    }
    
}

void ecs::ECS::printNumberOfComponents(Entity entity) {
    std::cout << "Entity: " << entity << ":" << std::endl;

    for (const auto& [typeIndex, entityMap] : components) {
        auto entityIt = entityMap.find(entity);
        if (entityIt != entityMap.end()) {
            // Liczba komponentów przypisanych do tego typu
            std::cout << "Type: " << typeIndex.name()  << std::endl;
        }
    }
}
