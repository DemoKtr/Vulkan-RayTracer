#pragma once
#include "View/vkImage/image.h"

namespace vkImage {

	struct TextureStorageInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
		std::vector<float> data;
		size_t size;

	};
	class TextureStorage {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		std::vector<float> data;
		//res
		vk::Image image;
		vk::DeviceMemory imageMemory;
		vk::ImageView imageView;
		vk::Sampler sampler;

		//Resource Descriptors
		vk::DescriptorSetLayout layout;
		vk::DescriptorSet descriptorSet;
		vk::DescriptorPool descriptorPool;

		vk::CommandBuffer commandBuffer;
		vk::Queue queue;

		size_t bufferSize;

		void populate();
		void make_view();
		void make_sampler();
		void make_descriptor_set();
	public:
		TextureStorage(TextureStorageInputChunk info);
		void sendData(std::vector<float>&);
		~TextureStorage();

	};

}