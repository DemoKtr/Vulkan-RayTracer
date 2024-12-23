#pragma once

#include "config.h"




namespace vkPrefab {
	
struct PrefabRednerInput {
	glm::ivec2 screenSize;
	vk::Instance instance;
	//physical device
	vk::PhysicalDevice physicalDevice;
	//logical device
	vk::Device device;
	//queues
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;
	vk::Queue computeQueue;
};
	extern PrefabRednerInput data;
}



