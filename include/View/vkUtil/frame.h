#pragma once
#include "config.h"


namespace vkUtil {
	class SwapChainFrame {

	public:
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		//Swapchain-type stuff
		vk::Image mainimage;
		vk::ImageView mainimageView;
		vk::Framebuffer mainframebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;


		vk::CommandBuffer commandBuffer;

		void destroy();

	};
}