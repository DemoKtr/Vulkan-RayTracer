#pragma once
#include "config.h"

namespace vkRenderStructs {
	struct ProjectionData {
		glm::mat4 projection;
	};

	struct ViewProjectionData {
		glm::mat4 projection;
		glm::mat4 view;
	};
	struct ViewData {
		glm::mat4 view;
	};
	ProjectionData getProjectionMatrix(vk::Extent2D swapchainExtent);

	ViewProjectionData getViewProjectionMatrix(int width, int height);
}