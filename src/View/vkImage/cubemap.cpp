#include "View/vkImage/cubemap.h"
#include <string>
#include <View/vkImage/stb_image.h>

#include <vector>
#include <View/vkImage/cubemapConverter.h>
#include <View/vkUtil/memory.h>
#include <View/vkInit/descrpitors.h>





vkImage::CubemapEctTexture::CubemapEctTexture(const std::string filename) {
    

	this->filename = filename;

    Load();
     
}


void vkImage::CubemapEctTexture::Load()
{


    const float* pImg = stbi_loadf(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pImg) {
        printf("Error loading '%s'\n", filename.c_str());
        exit(1);
    }

    Bitmap In(width, height, channels, eBitmapFormat_Float, (void*)pImg);
    std::vector<Bitmap> Cubemap;
    ConvertEquirectangularImageToCubemap(In, Cubemap);
    stbi_image_free((void*)pImg);
	width = Cubemap[0].w_;
	height = Cubemap[0].h_;
	this->cubemap = Cubemap;


}


void vkImage::CubemapEctTexture::LoadCubemapData(const TextureInputChunk& info) {

	device = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	filename = info.filenames;
	commandBuffer = info.commandBuffer;
	queue = info.queue;
	descriptorLayout = info.layout;
	descriptorPool = info.descriptorPool;

    ImageInputChunk imageInput;
    imageInput.logicalDevice = device;
    imageInput.physicalDevice = physicalDevice;
    imageInput.width = width;
    imageInput.height = height;
    imageInput.format = vk::Format::eR8G8B8A8Unorm;
    imageInput.arrayCount = this->cubemap.size();
    imageInput.tiling = vk::ImageTiling::eOptimal;
    imageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	imageInput.flags = vk::ImageCreateFlagBits::eCubeCompatible;
    image = make_image(imageInput);
    imageMemory = make_image_memory(imageInput, image);
	populate();
	make_view(this->cubemap.size());
	make_sampler();
	make_descriptor_set();

	

}

void vkImage::CubemapEctTexture::populate() {
	///color texture
	//First create a CPU-visible buffer...

	BufferInputChunk input;
	input.logicalDevice = device;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;

	size_t totalSize = 0;
	size_t sizeOfArray = 0;
	for (const auto& image : this->cubemap) {
		totalSize += width * height * 4; // assuming 4 bytes per pixel (e.g., RGBA format)
		//std::cout << "Hej jestem szerokoscia: " << image.w_ << " i wysokoscia: " << image.h_ << " arraya: " << sizeOfArray++ << std::endl;;
	}
	std::vector<uint8_t> mergedPixels;
	mergedPixels.reserve(totalSize);
	input.size = totalSize;

	// 3. Skopiuj wszystkie dane z pixels do mergedPixels
	for (const auto& image : this->cubemap) {
		for (const auto& value : image.data_) {
			mergedPixels.insert(mergedPixels.end(), value, value + width * height * channels);
		}
		
	}

	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = device.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	if (writeLocation) {
		memcpy(writeLocation, mergedPixels.data(), totalSize);
		device.unmapMemory(stagingBuffer.bufferMemory);
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
	transitionJob.arrayCount = this->cubemap.size();

	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = width;
	copyJob.height = height;
	copyJob.arrayCount = this->cubemap.size();
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(transitionJob);

	//Now the staging buffer can be destroyed
	device.freeMemory(stagingBuffer.bufferMemory);
	device.destroyBuffer(stagingBuffer.buffer);
}

void vkImage::CubemapEctTexture::make_view(int numberOfArray) {
	imageView = make_image_view(device, image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::eCube, numberOfArray);
}

void vkImage::CubemapEctTexture::make_sampler() {
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
		sampler = device.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}
}

void vkImage::CubemapEctTexture::make_descriptor_set() {
	
	

	descriptorSet = vkInit::allocate_descriptor_set(device, descriptorPool, descriptorLayout);

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
	device.updateDescriptorSets(descriptorWrite, nullptr);
}





void vkImage::CubemapEctTexture::Bind(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, int layoutIndex) {

    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, layoutIndex, descriptorSet, nullptr);
}