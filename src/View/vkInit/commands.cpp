#include "View/vkInit/commands.h"
#include <View/vkUtil/queues.h>

vk::CommandPool vkInit::make_command_pool(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, bool debugMode) {
	vkUtil::QueueFamilyIndices queueFamilyIndices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	vk::CommandPoolCreateInfo poolInfo = {};
	poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	try {
		return device.createCommandPool(poolInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode)
			std::cout << "Failed Create commandPool" << std::endl;
		return nullptr;
	}
}

vkInit::commandBufferOutput vkInit::make_command_buffer(commandBufferInputChunk inputChunk, bool debugMode) {
	vk::CommandBufferAllocateInfo allocInfo = {};
	allocInfo.commandPool = inputChunk.commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	commandBufferOutput output;
	try {
		output.graphicCommandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
		if (debugMode) std::cout << "Allocated main command buffer for frame" << std::endl;
		return output;


	}
	catch (vk::SystemError err) {
		std::cout << "FAILED!!! Allocated  main command buffer for frame" << std::endl;
		return output;
	}
}

void vkInit::make_frame_command_buffers(commandBufferInputChunk inputChunk, bool debugMode){
	vk::CommandBufferAllocateInfo allocInfo = {};
	allocInfo.commandPool = inputChunk.commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;

	vk::CommandBufferAllocateInfo igallocInfo = {};
	igallocInfo.commandPool = inputChunk.imguiCommandPool;
	igallocInfo.level = vk::CommandBufferLevel::ePrimary;
	igallocInfo.commandBufferCount = 1;



	for (int i = 0; i < inputChunk.frames.size(); ++i) {
		try {
			inputChunk.frames[i].commandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
			if (debugMode) std::cout << "Allocated Primary command buffer for frame" << i << std::endl;
			inputChunk.frames[i].imguiCommandBuffer = inputChunk.device.allocateCommandBuffers(igallocInfo)[0];
			if (debugMode) std::cout << "Allocated graphic particle command buffer for frame" << i << std::endl;
			
		}
		catch (vk::SystemError err) {
			std::cout << "FAILED!!! Allocated command buffer for frame" << i << std::endl;
		}
	}
}

