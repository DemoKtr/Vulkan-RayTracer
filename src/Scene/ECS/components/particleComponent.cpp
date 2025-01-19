#include "Scene/ECS/components/particleComponent.h"

ParticleComponent::ParticleComponent() {


}

vkParticle::ParticleEmmiter& ParticleComponent::getEmiter() {
	return emiter;
}

void ParticleComponent::update(float deltaTime) {
	emiter.update();
}

nlohmann::json ParticleComponent::serialize() const
{
	return nlohmann::json();
}
