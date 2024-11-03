#include "View/RenderStructs/projection.h"
#include <glm/gtc/matrix_transform.hpp>

vkRenderStructs::ProjectionData vkRenderStructs::getProjectionMatrix(vk::Extent2D swapchainExtent) {
	ProjectionData data;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1920) / static_cast<float>(1080), 0.1f, 1024.0f);
	projection[1][1] *= -1;
	data.projection = projection;
	return data;
}
