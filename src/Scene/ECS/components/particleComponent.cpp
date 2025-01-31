#include "Scene/ECS/components/particleComponent.h"

ParticleComponent::ParticleComponent()  {

	emiter = new vkParticle::ParticleEmmiter();
	type = ComponentType::Particle;
}

ParticleComponent::~ParticleComponent() {
	delete emiter;
}

vkParticle::ParticleEmmiter* ParticleComponent::getEmiter() {
	return emiter;
}

void ParticleComponent::update(float deltaTime) {
	emiter->update();
}

void ParticleComponent::write_flag_pointer(bool* ptr) {
	emiter->writeDirtyFlagAdress(ptr);
}

nlohmann::json ParticleComponent::serialize() const
{
	return nlohmann::json();
}
