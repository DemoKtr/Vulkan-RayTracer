#include "View/RenderStructs/projection.h"
#include <glm/gtc/matrix_transform.hpp>

vkRenderStructs::ProjectionData vkRenderStructs::getProjectionMatrix(vk::Extent2D swapchainExtent) {
	ProjectionData data;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1920) / static_cast<float>(1080), 0.1f, 1024.0f);
	projection[1][1] *= -1;
	data.projection = projection;
	return data;
}

vkRenderStructs::ViewProjectionData vkRenderStructs::getViewProjectionMatrix(int width, int height){
	
	ViewProjectionData data;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1920) / static_cast<float>(1080), 0.1f, 1024.0f);
	projection[1][1] *= -1;
	data.projection = projection;

	glm::vec3 eye = { 0.0f, 0.0f, -5.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);

	data.view = view;
	return data;
	
}
