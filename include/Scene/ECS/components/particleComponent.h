#pragma once
#include "Scene/ECS/component.h"
#include "View/vkParticle/ParticleEmitter.h"


class ParticleComponent : public Component {
    
    vkParticle::ParticleEmmiter* emiter;
public:
    ParticleComponent();
    ~ParticleComponent();
    vkParticle::ParticleEmmiter* getEmiter();
    void update(float deltaTime);
    void write_flag_pointer(bool* ptr);

    nlohmann::json serialize() const override;
};