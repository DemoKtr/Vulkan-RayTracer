#pragma once
#include "config.h"

namespace vkRenderStructs {
	struct ProjectionData {
		glm::mat4 projection;
	};

	ProjectionData getProjectionMatrix(vk::Extent2D swapchainExtent);
}