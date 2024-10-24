#pragma once
#include "glm/glm.hpp"

namespace vkGeometry{
	struct Vertex {
		glm::vec4 Position;
		glm::vec4 Normal;
		glm::vec2 TexCoords;
	};
}