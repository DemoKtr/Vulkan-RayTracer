#pragma once
#include "View/vkImage/image.h"

namespace vkImage {


	struct Texture3DInput {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
		int width;
		int height;
		int depth;
		std::vector<float> data;
	};

	class Texture3D {
		int width, height,depth ,channels;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;


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


		void populate();
		void make_view();
		void make_gray_scale_view();
		void make_view(vk::Format format, vk::ImageViewType type);
		void make_sampler();
		void make_descriptor_set();

	public:
		
		const char* filename;

		vk::DescriptorSet getDescriptorSet();
		vk::Sampler getSampler();

		Texture3D(Texture3DInput info);
		~Texture3D();

		void wrie_to_descriptor_set(vk::DescriptorSet& desc);
		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout,int layoutIndex);
		

	};
}