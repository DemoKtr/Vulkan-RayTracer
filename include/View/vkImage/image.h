#pragma once
#include "config.h"
#include "View/vkImage/stb_image.h"
#include "fileOperations/filesFinding.h"

namespace vkImage {

	struct TextureInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		fileOperations::filesPaths texturesNames;
		const char* filenames;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;

	};
	struct TextureDataInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
		int width;
		int height;
		std::vector<unsigned char*> data;
	};

	struct TextureFloatDataInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
		int width;
		int height;
		std::vector<float> data;
	};

	struct skyBoxTextureInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		std::vector<const char*> filenames;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
	};

	struct ImageInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		int width, height;
		vk::ImageTiling tiling;
		vk::ImageUsageFlags usage;
		vk::MemoryPropertyFlags memoryProperties;
		vk::Format format;
		uint32_t arrayCount;
		vk::ImageCreateFlags flags;
	};

	struct Image3DInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		int width, height,depth;
		vk::ImageTiling tiling;
		vk::ImageUsageFlags usage;
		vk::MemoryPropertyFlags memoryProperties;
		vk::Format format;
		uint32_t arrayCount;
		vk::ImageCreateFlags flags;
	};



	struct BufferImageCopyJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Buffer srcBuffer;
		vk::Image dstImage;
		int width, height; 
		int depth = 1.0f;
		uint32_t arrayCount;

	};

	struct ImageLayoutTransitionJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Image image;
		vk::ImageLayout oldLayout, newLayout;
		uint32_t arrayCount;
	};

	vk::Image make_image(ImageInputChunk input);
	vk::Image make_image(Image3DInputChunk input);

	vk::DeviceMemory make_image_memory(ImageInputChunk input, vk::Image image);
	vk::DeviceMemory make_image_memory(Image3DInputChunk input, vk::Image image);
	void transition_image_layout(ImageLayoutTransitionJob job);
	void copy_buffer_to_image(BufferImageCopyJob job);
	vk::ImageView make_image_view(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspect, vk::ImageViewType type, uint32_t arrayCount);

	vk::Format find_supported_format(
		vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features
	);






}


