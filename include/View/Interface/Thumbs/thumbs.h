#pragma once
#include "config.h"


namespace vkThumbs {

	struct ThumbInput {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
	};

		class Thumb {
		protected:
			vk::Device device;
			vk::PhysicalDevice physicalDevice;
			vk::CommandBuffer commandBuffer;
			vk::Queue queue;
			vk::DescriptorSetLayout layout;
			vk::DescriptorPool descriptorPool;
		
			Thumb(ThumbInput input);

	};
}
