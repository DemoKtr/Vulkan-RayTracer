#include "View/vkParticle/ParticleEmitter.h"

vkParticle::ParticleEmmiter::ParticleEmmiter() {
   
	
	//generators[0].GenerateParticles();

}

vkParticle::ParticleEmmiter::~ParticleEmmiter() {
  
}

void vkParticle::ParticleEmmiter::writeDirtyFlagAdress(bool* ptr) {
	dirtyflag = ptr;
	ParticleGenerator  gen(dirtyflag);
	AddGenerator(gen);
	generators[0].update();
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

	

}

void vkParticle::ParticleEmmiter::AddGenerator(const vkParticle::ParticleGenerator& generator) {
	generators.push_back(generator);
	*dirtyflag = true;
}

bool vkParticle::ParticleEmmiter::RemoveGenerator(size_t index) {
	if (index < generators.size()) {
		generators.erase(generators.begin() + index);
		*dirtyflag = true;
		return true; // Sukces
	}
	else {
		return false; // Niepoprawny indeks
	}
}


