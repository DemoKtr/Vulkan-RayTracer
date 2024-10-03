#pragma once
#include "config.h"

namespace vkGeometry{
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		glm::vec3 Tangent;
	};
}