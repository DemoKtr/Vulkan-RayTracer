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
}
