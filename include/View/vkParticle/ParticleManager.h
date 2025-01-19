#pragma once
#include "config.h"
#include <Scene/sceneObject.h>
#include <Scene/ECS/components/components.h>


namespace vkParticle {
    class ParticleManager {
    public:
        // Konstruktor
        ParticleManager();
        ~ParticleManager();

        // Dodaj komponent cz¹steczek do mened¿era
        void AddParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject);
        void AddParticleComponent();
    

        // Usuñ komponent cz¹steczek z mened¿era
        void RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject);

        // Aktualizuj wszystkie komponenty cz¹steczek
        void Update(float deltaTime, void* memory, std::vector<vkParticle::Particle>& particles);

        // Renderuj wszystkie komponenty cz¹steczek
        void Render();

        // Usuñ wszystkie komponenty
        void Clear() {
            for (vkParticle::ParticleEmmiter* emiter : particleEmiters) {
                delete emiter;
            }
        }

    private:
        std::vector<vkParticle::ParticleEmmiter*> particleEmiters;
    };
}