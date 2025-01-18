#pragma once
#include "View/vkParticle/Particle.h"
#include "View/vkParticle/ParticleGenerator.h"
#include "config.h"
namespace vkParticle {
	class ParticleEmmiter {
		//EmitterType type;
		bool* dirtyflag;
		std::vector<ParticleGenerator> generators;
		glm::vec4 postion;
		size_t ParticleCount;
		size_t MaxParticleView;
		

	public:
		ParticleEmmiter();
		~ParticleEmmiter();


		std::vector<ParticleGenerator>& getGenerators();
		ParticleGenerator& getGenerator(int index);


		void update();

		void AddGenerator(const vkParticle::ParticleGenerator& generator);
		bool RemoveGenerator(size_t index);

	};
}