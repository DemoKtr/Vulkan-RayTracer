#pragma once
#include "config.h"
#include <Scene/sceneObject.h>
#include <Scene/ECS/components/components.h>


namespace vkParticle {
    class ParticleManager {
    public:
        // Konstruktor
        ParticleManager() = default;

        // Dodaj komponent cz¹steczek do mened¿era
        void AddParticleComponent(ecs::ECS* ecs,SceneObject* sceneObject) {
            if (sceneObject && ecs->hasComponent<ParticleComponent>(sceneObject->id)) {
                particleComponents.push_back(ecs->getComponent<ParticleComponent>(sceneObject->id));
            }
        }

        // Usuñ komponent cz¹steczek z mened¿era
        void RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
            if (!sceneObject) return;

            auto it = std::remove_if(particleComponents.begin(), particleComponents.end(),
                [&](std::shared_ptr<ParticleComponent> component) {
                    return component == ecs->getComponent<ParticleComponent>(sceneObject->id);
                });

            if (it != particleComponents.end()) {
                particleComponents.erase(it, particleComponents.end());
            }
        }

        // Aktualizuj wszystkie komponenty cz¹steczek
        void Update(float deltaTime) {
            for (auto& component : particleComponents) {
                if (component) {
                    component->update(deltaTime);
                }
            }
        }

        // Renderuj wszystkie komponenty cz¹steczek
        void Render() {
           
        }

        // Usuñ wszystkie komponenty
        void Clear() {
            particleComponents.clear();
        }

    private:
        std::vector<std::shared_ptr<ParticleComponent>> particleComponents;
    };
}