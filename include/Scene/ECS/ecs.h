#pragma once

#include <unordered_map>
#include <vector>
#include <bitset>
#include <typeindex>
#include <memory>

#include "Scene/ECS/entity.h"
#include "Scene/ECS/system.h"
#include "Scene/ECS/component.h"

namespace ecs {
    class ECSSystem;
    class ECS {
    public:
        Entity createEntity();

        template <typename T>
        void addComponent(Entity entity, std::shared_ptr<T> component) {
            const std::type_index typeIndex = std::type_index(typeid(T));
            components[typeIndex][entity] = component;
            entityMasks[entity].set(componentBitIndex<T>(), true);
        }

        template <typename T>
        bool hasComponent(Entity entity) const {
            return entityMasks.at(entity).test(componentBitIndex<T>());
        }

        template <typename T>
        std::shared_ptr<T> getComponent(Entity entity) const {
            const std::type_index typeIndex = std::type_index(typeid(T));
            return std::static_pointer_cast<T>(components.at(typeIndex).at(entity));
        }

        const std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>>& getEntityMasks() const;

        void updateSystems();

        void addSystem(std::shared_ptr<ECSSystem> system);

    private:
        Entity nextEntityId = 1;
        std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> entityMasks;
        std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>> components;
        std::vector<std::shared_ptr<ECSSystem>> systems;

        template <typename T>
        std::size_t componentBitIndex() const {
            static std::size_t index = nextComponentBitIndex++;
            return index;
        }

        static inline std::size_t nextComponentBitIndex = 0;
    };

}