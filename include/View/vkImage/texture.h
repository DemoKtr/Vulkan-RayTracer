#pragma once
#include "View/vkImage/image.h"

namespace vkImage {


	class Texture {
		int width, height, channels;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		std::vector<stbi_uc*> pixels;
		std::vector<float> data;
		fileOperations::filesPaths texturesNames;
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
		void send_data();
		void make_view();
		void make_gray_scale_view();
		void make_view(vk::Format format, vk::ImageViewType type);
		void make_sampler();
		void make_descriptor_set();

	public:
		
		const char* filename;

		vk::DescriptorSet getDescriptorSet();
		vk::Sampler getSampler();

		Texture(TextureInputChunk info);
		Texture(TextureDataInputChunk info);
		Texture(vkImage::TextureFloatDataInputChunk& info);
		~Texture();

		void wrie_to_descriptor_set(vk::DescriptorSet& desc);
		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout);
		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout,int layoutIndex);
		

	};
}