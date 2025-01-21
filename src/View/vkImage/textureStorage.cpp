#include "View/vkImage/textureStorage.h"
#include <View/vkUtil/memory.h>

void vkImage::TextureStorage::populate() {
	//color texture
	//First create a CPU-visible buffer...

	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;
	
	input.size = data.size() * sizeof(float);



	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	if (writeLocation) {
		memcpy(writeLocation, data.data(), input.size);
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
	transitionJob.newLayout = vk::ImageLayout::eGeneral;
	transitionJob.arrayCount = 1;

	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = data.size();
	copyJob.height = 1;
	copyJob.arrayCount = 1;
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eGeneral;
	transitionJob.newLayout = vk::ImageLayout::eGeneral;
	transition_image_layout(transitionJob);

	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
}

void vkImage::TextureStorage::make_view() {
	imageView = make_image_view(logicalDevice, image, vk::Format::eR32G32B32A32Sfloat, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e1D, 1);
}

void vkImage::TextureStorage::make_sampler() {
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

void vkImage::TextureStorage::make_descriptor_set() {
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

vkImage::TextureStorage::TextureStorage(TextureStorageInputChunk info) {
	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	commandBuffer = info.commandBuffer;
	queue = info.queue;
	layout = info.layout;
	descriptorPool = info.descriptorPool;
	bufferSize = info.size;

	data = info.data;

	ImageInputChunk imageInput;
	imageInput.logicalDevice = logicalDevice;
	imageInput.physicalDevice = physicalDevice;
	imageInput.width = bufferSize;
	imageInput.height = 1;
	imageInput.format = vk::Format::eR32G32B32A32Sfloat;
	imageInput.arrayCount = 1;
	imageInput.tiling = vk::ImageTiling::eOptimal;
	imageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	image = make_image(imageInput);
	imageMemory = make_image_memory(imageInput, image);

	if(bufferSize>0)
	populate();

	data.clear();


	make_view();

	make_sampler();

	make_descriptor_set();
}

void vkImage::TextureStorage::sendData(std::vector<float>&) {

	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;

	input.size = data.size() * sizeof(float);



	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	if (writeLocation) {
		memcpy(writeLocation, data.data(), input.size);
		logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	}
	else {
		std::cerr << "Memory mapping failed!" << std::endl;
	}

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = data.size();
	copyJob.height = 1;
	copyJob.arrayCount = 1;
	copy_buffer_to_image(copyJob);

	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);

}

vkImage::TextureStorage::~TextureStorage() {
	logicalDevice.freeMemory(imageMemory);
	logicalDevice.destroyImage(image);
	logicalDevice.destroyImageView(imageView);
	logicalDevice.destroySampler(sampler);
}
