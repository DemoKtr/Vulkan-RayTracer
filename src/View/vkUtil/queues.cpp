#include "View/vkUtil/queues.h"
vkUtil::QueueFamilyIndices vkUtil::findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode)
{
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
	if (debugMode) {
		std::cout << "There are " << queueFamilies.size() << "supported" << std::endl;
	}

	int i{ 0 };
	for (vk::QueueFamilyProperties queueFamily : queueFamilies) {

		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics && !(indices.graphicsFamily.has_value())) {
				indices.graphicsFamily = i;
				if (debugMode) {
					std::cout << "Queue Family " << i << " is suitable for graphics and have"<<queueFamilies[i].queueCount <<"queues " << std::endl;


				}
		}
		if (device.getSurfaceSupportKHR(i, surface)&& !(indices.presentFamily.has_value())) {
			indices.presentFamily = i;
			if (debugMode) {
				std::cout << "Queue Family " << i << "is suitable for present and have" << queueFamilies[i].queueCount << "queues " << std::endl;
			}
		}
		if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute && (indices.presentFamily.value() != i && indices.graphicsFamily.value() != i) && !(indices.computeFamily.has_value())) {
			indices.computeFamily = i;
			if (debugMode) {
				std::cout << "Queue Family " << i << "is suitable for compute and have" << queueFamilies[i].queueCount << "queues " << std::endl;
			}
		}

		if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer && (indices.presentFamily.value() != i && indices.graphicsFamily.value() != i && indices.computeFamily.value() != i)) {
			indices.transferFamily = i;
			if (debugMode) {
				std::cout << "Queue Family " << i << " is suitable for transfer and have" << queueFamilies[i].queueCount << "queues " << std::endl;


			}
		}

		if (indices.isComplete()) {
			break;
		}
		i++;
	}

	return indices;
}