#pragma once
#include "config.h"

namespace vkExtensions{
	static PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR = nullptr;
	static PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = nullptr;
	static PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;
	static PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
	static PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
	static PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = nullptr;
	static PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR = nullptr;
	static PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR = nullptr;
	static PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR = nullptr;
	static PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR = nullptr; 


	void Init(vk::Device device);

	uint64_t getBufferDeviceAddress(vk::Device device, vk::Buffer buffer);
}