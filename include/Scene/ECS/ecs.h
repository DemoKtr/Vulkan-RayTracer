#pragma once

#include <unordered_map>
#include <vector>
#include <bitset>
#include <typeindex>
#include <memory>
#include <stdexcept>

#include "Scene/ECS/entity.h"
#include "Scene/ECS/system.h"
#include "Scene/ECS/component.h"
#include <iostream>
namespace ecs {
    class ECSSystem;
    class ECS {
    public:
        ECS();
        Entity createEntity();

        template <typename T>
        void addComponent(Entity entity ,std::shared_ptr<T> component) {
            const std::type_index typeIndex = std::type_index(typeid(T));

            // Jeœli dla danego typu nie ma wpisu, utwórz nowy
            if (components[typeIndex].find(entity) == components[typeIndex].end()) {
                components[typeIndex][entity] = std::vector<std::shared_ptr<Component>>();
            }

            // Dodaj nowy komponent do listy komponentów dla danego entity
            components[typeIndex][entity].push_back(component);
            //flag = true;
            // Zaktualizuj maskê komponentów
            entityMasks[entity].set(componentBitIndex<T>(), true);
        }

        template <typename T>
        bool hasComponent(Entity entity) const {

            auto it = entityMasks.find(entity);
            if (it != entityMasks.end()) {

                return it->second.test(componentBitIndex<T>());
            }
            std::cout << "Entity not found in masks!" << std::endl;  // Debugging line
            return false;
        }

        template <typename T>
        std::shared_ptr<T> getComponent(Entity entity) const {
            const std::type_index typeIndex = std::type_index(typeid(T));

            // SprawdŸ, czy istnieje komponent danego typu
            if (components.find(typeIndex) == components.end()) {
                //throw std::out_of_range("No components of the requested type found");

                return nullptr;
            }

            const auto& entityComponents = components.at(typeIndex);

            // SprawdŸ, czy istnieje komponent dla tego entity
            if (entityComponents.find(entity) == entityComponents.end()) {
                //throw std::out_of_range("Entity does not have components of the requested type");

                return nullptr;
            }

            const auto& componentVector = entityComponents.at(entity);

            // SprawdŸ, czy s¹ komponenty w wektorze
            if (componentVector.empty()) {
                //throw std::out_of_range("No components available for the entity

                return nullptr;
            }

            // Zak³adamy, ¿e interesuje nas pierwszy komponent
            return std::static_pointer_cast<T>(componentVector[0]);
        }


        const std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>>& getEntityMasks() const;

        void updateSystems();

        void addSystem(std::shared_ptr<ECSSystem> system);

        std::vector<std::shared_ptr<Component>> getAllComponents(Entity entity) const;

        void removeComponent(Component* component, Entity entity);
        void printNumberOfComponents(Entity entity);

        std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> entityMasks;
        std::unordered_map<std::type_index, std::unordered_map<Entity, std::vector<std::shared_ptr<Component>>>> components;
    private:
        Entity nextEntityId = 1;
        
        std::vector<std::shared_ptr<ECSSystem>> systems;

        template <typename T>
        std::size_t componentBitIndex() const {
            static std::size_t index = nextComponentBitIndex++;
            return index;
        }

        static inline std::size_t nextComponentBitIndex = 0;
    };

}