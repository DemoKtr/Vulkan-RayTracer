#pragma once
#include "config.h"


namespace vkParticle {
	struct Particle {
        glm::vec4 position = glm::vec4(0.0f);
        glm::vec4 velocity = glm::vec4(0.0f);
        glm::vec4 acceleration = glm::vec4(0.0f);
        glm::vec4 size = glm::vec4(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
        glm::vec4 lifeRotationRSpeed = glm::vec4(0.0f);
        glm::uvec4 textureIndex = glm::uvec4(1);
	};

    struct ParticleInit {
        glm::vec4 position = glm::vec4(0.0f);
        glm::vec4 velocity = glm::vec4(0.0f);
        glm::vec4 acceleration = glm::vec4(0.0f);
        glm::vec4 size = glm::vec4(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
        glm::vec4 lifeRotationRSpeed = glm::vec4(0.0f);


        ParticleInit() {
            this->position = glm::vec4(0.0f);
            this->velocity = glm::vec4(0.0f);
            this->acceleration = glm::vec4(0.0f);
            this->size = glm::vec4(0.0f);
            this->color = glm::vec4(0.0f);
            this->lifeRotationRSpeed = glm::vec4(0.0f);
        }

        ParticleInit(Particle& particle) {
            this->position = particle.position;
            this->velocity = particle.velocity;
            this->acceleration = particle.acceleration;
            this->size = particle.size;
            this->color = particle.color;
            this->lifeRotationRSpeed = particle.lifeRotationRSpeed;

        }
    };
}
