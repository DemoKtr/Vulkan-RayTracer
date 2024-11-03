#pragma once
#include "config.h"
#include <View/vkVertexData/vertex.h>

namespace vkMesh {
	vk::VertexInputBindingDescription getVertexInputBindingDescription();


	std::vector<vk::VertexInputAttributeDescription> getVertexInputAttributeDescription();

}