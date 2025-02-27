#include "View/vkInit/attachment.h"
#include <View/vkUtil/memory.h>

void vkInit::create_attachment(vk::PhysicalDevice physicalDevice,vk::Device logicalDevice,vk::Format f, vk::ImageUsageFlagBits u,vk::Extent2D swapchainExten,vk::Image& image, vk::DeviceMemory& mem, vk::ImageView& view) {
	vk::ImageAspectFlags aspectMask(0);
	vk::ImageLayout imageLayout;
	vk::Format format = f;
	if (u & vk::ImageUsageFlagBits::eColorAttachment) {
		aspectMask = vk::ImageAspectFlagBits::eColor;
		imageLayout = vk::ImageLayout::eColorAttachmentOptimal ;
	}
	if (u & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (f >= vk::Format::eD16UnormS8Uint) {
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
		imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	}

	assert(static_cast<uint32_t>(aspectMask) > 0);
	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.format = f;
	imageInfo.extent.width = swapchainExten.width;
	imageInfo.extent.height = swapchainExten.height;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.extent.depth = 1;
	imageInfo.usage = u | vk::ImageUsageFlagBits::eSampled;
	
	image = logicalDevice.createImage(imageInfo);
	vk::MemoryRequirements requirements = logicalDevice.getImageMemoryRequirements(image);
	vk::MemoryAllocateInfo allocation;
	allocation.allocationSize = requirements.size;
	allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(physicalDevice, requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
	mem = logicalDevice.allocateMemory(allocation);
	logicalDevice.bindImageMemory(image, mem, 0);

	vk::ImageViewCreateInfo  imageViewInfo = {};
	imageViewInfo.viewType = vk::ImageViewType::e2D;
	imageViewInfo.format = format;
	imageViewInfo.format = format;
	imageViewInfo.subresourceRange;
	imageViewInfo.subresourceRange.aspectMask = aspectMask;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;
	imageViewInfo.image = image;
	
	view = logicalDevice.createImageView(imageViewInfo);
	
}
