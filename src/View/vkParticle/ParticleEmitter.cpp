#include "View/vkParticle/ParticleEmitter.h"

vkParticle::ParticleEmmiter::ParticleEmmiter() {
    dirtyflag = new bool(true);
	AddGenerator(dirtyflag);
	generators[0].update();
	generators[0].GenerateParticles();

}

vkParticle::ParticleEmmiter::~ParticleEmmiter() {
    delete dirtyflag;
}

std::vector<vkParticle::ParticleGenerator>& vkParticle::ParticleEmmiter::getGenerators()
{
    return this->generators;
}

vkParticle::ParticleGenerator& vkParticle::ParticleEmmiter::getGenerator(int index)
{
    return generators[index];
}



void vkParticle::ParticleEmmiter::update() {
	if (dirtyflag) {


	}
}

void vkParticle::ParticleEmmiter::AddGenerator(const vkParticle::ParticleGenerator& generator) {
	generators.push_back(generator);
		*dirtyflag = true;
}

bool vkParticle::ParticleEmmiter::RemoveGenerator(size_t index) {
	if (index < generators.size()) {
		generators.erase(generators.begin() + index);
		return true; // Sukces
	}
	else {
		return false; // Niepoprawny indeks
	}
}


