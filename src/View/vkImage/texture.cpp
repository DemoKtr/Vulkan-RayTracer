#include "View/vkImage/texture.h"
#include <View/vkUtil/memory.h>
#include <View/vkInit/descrpitors.h>

void vkImage::Texture::load() {
	size_t i = 0;
	if (filename == nullptr) {
		for (std::string f : texturesNames.fileNames) {
			pixels.push_back(stbi_load(texturesNames.fullPaths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha));
			if (!pixels[i++]) {
				std::cout << "Unable to load: " << f << std::endl;
			}
		}
	}
	else {
		pixels.push_back(stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha));
		if (!pixels[i++]) {
			std::cout << "Unable to load: " << filename << std::endl;
		}
	}
	
	
}


void vkImage::Texture::populate() {
	///color texture
	//First create a CPU-visible buffer...
	
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;
	size_t totalSize = 0;
	for (const auto& image : pixels) {
		totalSize += width * height * 4; // assuming 4 bytes per pixel (e.g., RGBA format)
	}
	std::vector<stbi_uc> mergedPixels;
	mergedPixels.reserve(totalSize);
	input.size = totalSize;

	// 3. Skopiuj wszystkie dane z pixels do mergedPixels
	for (const auto& image : pixels) {
		mergedPixels.insert(mergedPixels.end(), image, image + width * height * 4);
	}

	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	if (writeLocation) {
		memcpy(writeLocation, mergedPixels.data(), totalSize);
		logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	}
	else {
		std::cerr << "Memory mapping failed!" << std::endl;
	}

	//then transfer it to image memory
	ImageLayoutTransitionJob transitionJob;
	transitionJob.commandBuffer = commandBuffer;
	transitionJob.queue = queue;
	transitionJob.image = image;
	transitionJob.oldLayout = vk::ImageLayout::eUndefined;
	transitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.arrayCount = pixels.size();

	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = width;
	copyJob.height = height;
	copyJob.arrayCount = pixels.size();
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(transitionJob);

	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);

}

void vkImage::Texture::make_view(){
	if (filename == nullptr) {
		imageView = make_image_view(logicalDevice, image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2DArray, pixels.size());
	}
	else {
	imageView = make_image_view(logicalDevice, image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, pixels.size());
	}
	
}

void vkImage::Texture::make_sampler() {	/*
	typedef struct VkSamplerCreateInfo {
		VkStructureType         sType;
		const void* pNext;
		VkSamplerCreateFlags    flags;
		VkFilter                magFilter;
		VkFilter                minFilter;
		VkSamplerMipmapMode     mipmapMode;
		VkSamplerAddressMode    addressModeU;
		VkSamplerAddressMode    addressModeV;
		VkSamplerAddressMode    addressModeW;
		float                   mipLodBias;
		VkBool32                anisotropyEnable;
		float                   maxAnisotropy;
		VkBool32                compareEnable;
		VkCompareOp             compareOp;
		float                   minLod;
		float                   maxLod;
		VkBorderColor           borderColor;
		VkBool32                unnormalizedCoordinates;
	} VkSamplerCreateInfo;
	*/
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.flags = vk::SamplerCreateFlags();
	samplerInfo.minFilter = vk::Filter::eNearest;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;

	samplerInfo.anisotropyEnable = false;
	samplerInfo.maxAnisotropy = 1.0f;

	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = false;
	samplerInfo.compareEnable = false;
	samplerInfo.compareOp = vk::CompareOp::eAlways;

	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	try {
		sampler = logicalDevice.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}
}

void vkImage::Texture::make_descriptor_set() {

	descriptorSet = vkInit::allocate_descriptor_set(logicalDevice, descriptorPool, layout);

	vk::DescriptorImageInfo imageDescriptor;
	imageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptor.imageView = imageView;
	imageDescriptor.sampler = sampler;

	vk::WriteDescriptorSet descriptorWrite;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageDescriptor;
	logicalDevice.updateDescriptorSets(descriptorWrite, nullptr);

}

vk::DescriptorSet vkImage::Texture::getDescriptorSet(){
	return descriptorSet;
}

vk::Sampler vkImage::Texture::getSampler()
{
	return sampler;
}

vkImage::Texture::Texture(TextureInputChunk info) {

	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	texturesNames = info.texturesNames;
	filename = info.filenames;
	commandBuffer = info.commandBuffer;
	queue = info.queue;
	layout = info.layout;
	descriptorPool = info.descriptorPool;

	load();
	
	ImageInputChunk imageInput;
	imageInput.logicalDevice = logicalDevice;
	imageInput.physicalDevice = physicalDevice;
	imageInput.width = width;
	imageInput.height = height;
	imageInput.format = vk::Format::eR8G8B8A8Unorm;
	imageInput.arrayCount =pixels.size();
	imageInput.tiling = vk::ImageTiling::eOptimal;
	imageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	image = make_image(imageInput);
	imageMemory = make_image_memory(imageInput, image);
	
	populate();

	for (stbi_uc* pix : pixels) {
		free(pix);
	}
	

	make_view();

	make_sampler();

	make_descriptor_set();

}

vkImage::Texture::~Texture() {
	logicalDevice.freeMemory(imageMemory);
	logicalDevice.destroyImage(image);
	logicalDevice.destroyImageView(imageView);
	logicalDevice.destroySampler(sampler);
}

void vkImage::Texture::useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout) {
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 1, descriptorSet, nullptr);
}
