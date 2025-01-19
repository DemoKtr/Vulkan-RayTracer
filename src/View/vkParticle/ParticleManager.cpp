#include "View/vkParticle/ParticleManager.h"

vkParticle::ParticleManager::ParticleManager() {
    AddParticleComponent();
}

vkParticle::ParticleManager::~ParticleManager() {
    Clear();
}

void vkParticle::ParticleManager::AddParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    
}

void vkParticle::ParticleManager::AddParticleComponent() {
   // particleComponents.push_back(vkParticle::ParticleGenerator());
    particleEmiters.push_back(new vkParticle::ParticleEmmiter);
}

void vkParticle::ParticleManager::RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    
}

void vkParticle::ParticleManager::Update(float deltaTime, void* memory, std::vector<vkParticle::Particle>& particles) {
    size_t i = 0;

    for (vkParticle::ParticleEmmiter* emitter : particleEmiters) {
        for (vkParticle::ParticleGenerator& generator : emitter->getGenerators()) {
            
            for (vkParticle::Particle& particle : generator.GetParticles()) {
                if (i == 1)std::cout << "X: " << particle.position.x << "Y: " << particle.position.y << "Z: " << particle.position.z << std::endl;
                if (i == 2)std::cout << "X: " << particle.position.x << "Y: " << particle.position.y << "Z: " << particle.position.z << std::endl;
                if (i == 3)std::cout << "X: " << particle.position.x << "Y: " << particle.position.y << "Z: " << particle.position.z << std::endl;
                particles[i++] = particle;

            }
        }
    }

    memcpy(memory, particles.data(), i * sizeof(vkParticle::Particle));
}


void vkParticle::ParticleManager::Render() {
    
}
