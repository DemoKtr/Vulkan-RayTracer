#pragma once
#include "config.h"


namespace vkParticle {
	struct Particle {
        glm::vec4 position = glm::vec4(0.0f);
        glm::vec4 velocity = glm::vec4(0.0f);
        glm::vec4 acceleration = glm::vec4(0.0f);
        glm::vec4 size = glm::vec4(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
        float lifetime = 0.0f;
        float rotation = 0.0f;
        float rotationSpeed = 0.0f;
        uint32_t textureIndex = 1;
	};
}
