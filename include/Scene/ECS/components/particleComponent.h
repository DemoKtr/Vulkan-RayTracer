#pragma once
#include "Scene/ECS/component.h"
#include "View/vkParticle/ParticleEmitter.h"


class ParticleComponent : public Component {
    
    vkParticle::ParticleEmmiter emiter;
public:
    ParticleComponent();
    vkParticle::ParticleEmmiter& getEmiter();
    void update(float deltaTime);

    nlohmann::json serialize() const override;
};