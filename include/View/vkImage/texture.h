#pragma once
#include "View/vkImage/image.h"

namespace vkImage {

	struct TextureInput {
		std::vector<std::string> texture_name;
		std::vector<std::string> texture_path;
	};



	class Texture {
		int width, height, channels;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		stbi_uc* pixels;
		
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

		void load();
		void populate();
		void make_view();
		void make_sampler();
		void make_descriptor_set();

	public:
		
		const char* filename;

		vk::DescriptorSet getDescriptorSet();
		vk::Sampler getSampler();

		Texture(TextureInputChunk info);

		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout);
		~Texture();

	};
}