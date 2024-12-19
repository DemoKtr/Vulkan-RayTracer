#pragma once
#include "View/vkInit/vkAccelerationStructures/GPUAdress.h"

uint64_t getBufferDeviceAddress(vk::Buffer buffer) {
	vk::BufferDeviceAddressInfoKHR bufferDeviceAI{};
	bufferDeviceAI.sType = vk::StructureType::eBufferDeviceAddressInfoKHR;
	//return vkGetBufferDeviceAddressKHR(vulkanDevice->logicalDevice, &bufferDeviceAI);
}

