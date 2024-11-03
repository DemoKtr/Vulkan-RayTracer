#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>

namespace vkInit {
	struct commandBufferInputChunk {
		vk::Device device;
		vk::CommandPool commandPool;
		vk::CommandPool imguiCommandPool;
		std::vector<vkUtil::SwapChainFrame>& frames;
	};

	struct commandBufferOutput {
		vk::CommandBuffer graphicCommandBuffer;
		vk::CommandBuffer computeCommandBuffer;
	};

	vk::CommandPool make_command_pool(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, bool debugMode);

	commandBufferOutput make_command_buffer(commandBufferInputChunk inputChunk, bool debugMode);

	

	void make_imgui_frame_command_buffers(commandBufferInputChunk inputChunk, bool debugMode);
	void make_postprocess_frame_command_buffers(commandBufferInputChunk inputChunk, bool debugMode);

}