#include "View/vkUtil/frame.h"
#include <View/vkUtil/memory.h>
#include <View/vkImage/image.h>
#include "MultithreatedSystems/mutexManager.h"
void vkUtil::SwapChainFrame::make_depth_resources() {
	depthFormat = vkImage::find_supported_format(physicalDevice, { vk::Format::eD32Sfloat,vk::Format::eD24UnormS8Uint }, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

	vkImage::ImageInputChunk imageInfo;
	imageInfo.logicalDevice = logicalDevice;
	imageInfo.physicalDevice = physicalDevice;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
	imageInfo.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	imageInfo.width = width;
	imageInfo.height = height;
	imageInfo.format = depthFormat;
	imageInfo.arrayCount = 1;
	depthBuffer = vkImage::make_image(imageInfo);
	depthBufferMemory = vkImage::make_image_memory(imageInfo, depthBuffer);
	depthBufferView = vkImage::make_image_view(
		logicalDevice, depthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
	);


	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.flags = vk::SamplerCreateFlags();
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;

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

void vkUtil::SwapChainFrame::make_descriptors_resources(int number_of_objects) {
	
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	input.physicalDevice = physicalDevice;
	input.size = sizeof(CameraUBO);
	input.usage = vk::BufferUsageFlagBits::eUniformBuffer;
	cameraDataBuffer = createBuffer(input);
	cameraDataWriteLocation = logicalDevice.mapMemory(cameraDataBuffer.bufferMemory,0,sizeof(CameraUBO));

	cameraUBODescriptor.buffer = cameraDataBuffer.buffer;
	cameraUBODescriptor.offset = 0;
	cameraUBODescriptor.range = sizeof(CameraUBO);

	input.size = sizeof(glm::mat4);
	particleUBODataBuffer = createBuffer(input);
	particleUBODataWriteLocation = logicalDevice.mapMemory(particleUBODataBuffer.bufferMemory, 0, sizeof(glm::mat4));

	ParticleUBODescriptor.buffer = particleUBODataBuffer.buffer;
	ParticleUBODescriptor.offset = 0;
	ParticleUBODescriptor.range = sizeof(glm::mat4);

	
	input.size = sizeof(DtSBO);
	DeltaTimeDataBuffer = createBuffer(input);
	DeltaTimeDataWriteLocation = logicalDevice.mapMemory(DeltaTimeDataBuffer.bufferMemory, 0, sizeof(DtSBO));

	DeltaTimeDescriptor.buffer = DeltaTimeDataBuffer.buffer;
	DeltaTimeDescriptor.offset = 0;
	DeltaTimeDescriptor.range = sizeof(DtSBO);

	input.size = sizeof(postprocess::FogUBO);
	fogUBODataBuffer = createBuffer(input);
	fogUBODataWriteLocation = logicalDevice.mapMemory(fogUBODataBuffer.bufferMemory, 0, sizeof(postprocess::FogUBO));

	fogUBODescriptor.buffer = fogUBODataBuffer.buffer;
	fogUBODescriptor.offset = 0;
	fogUBODescriptor.range = sizeof(postprocess::FogUBO);
	
	int sbo_size = number_of_objects+2048;
	input.size = sbo_size * sizeof(MeshSBO);
	input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
	modelsDataBuffer = createBuffer(input);
	modelsDataWriteLocation = logicalDevice.mapMemory(modelsDataBuffer.bufferMemory,0, sbo_size *sizeof(MeshSBO));


	input.size = 1024 * sizeof(glm::vec4);
	UIPositionSizeDataBuffer = createBuffer(input);
	UIPositionSizeDataWriteLocation = logicalDevice.mapMemory(UIPositionSizeDataBuffer.bufferMemory, 0, 1024 * sizeof(glm::vec4));



	input.size = 2048 * sizeof(FontSBO);
	UIFontPositionSizeDataBuffer = createBuffer(input);
	UIFontPositionSizeDataWriteLocation = logicalDevice.mapMemory(UIFontPositionSizeDataBuffer.bufferMemory, 0, 2048 * sizeof(FontSBO));

	

	//dt = glm::vec4(1.0f);

	modelsData.reserve(sbo_size);

	for (uint32_t i = 0; i < sbo_size; ++i) {
		MeshSBO meshSBO;
		meshSBO.model = glm::mat4(1.0f);
		meshSBO.textureID = -1;
		meshSBO.padding[0] = -1;
		meshSBO.padding[1] = -1;
		meshSBO.padding[2] = -1;
		
		modelsData.push_back(meshSBO);
	}

	UIPositionSize.reserve(1024);

	for (uint32_t i = 0; i < 1024; ++i) {
		
		UIPositionSize.push_back(glm::vec4(1.0f));
	}

	UIFontPositionSize.reserve(2048);

	for (uint32_t i = 0; i < 2048; ++i) {
		FontSBO font;
		font.PosSize = glm::vec4(1.0f);
		font.UVBounds = glm::vec4(1.0f);
		font.textures = glm::uvec4(1);
		UIFontPositionSize.push_back(font);
		
	}
	


	viewProjection = glm::mat4(1.0f);

	modelsSBODescriptor.buffer = modelsDataBuffer.buffer;
	modelsSBODescriptor.offset = 0;
	modelsSBODescriptor.range = sbo_size *sizeof(MeshSBO);

	UIPositionSizeDescriptor.buffer = UIPositionSizeDataBuffer.buffer;
	UIPositionSizeDescriptor.offset = 0;
	UIPositionSizeDescriptor.range = 1024 * sizeof(glm::vec4);

	UIFontPositionSizeDescriptor.buffer = UIFontPositionSizeDataBuffer.buffer;
	UIFontPositionSizeDescriptor.offset = 0;
	UIFontPositionSizeDescriptor.range = 2048 * sizeof(FontSBO);

	

}

void vkUtil::SwapChainFrame::write_postprocess_descriptors() {
	vk::WriteDescriptorSet writeInfo;
	/*
	typedef struct VkWriteDescriptorSet {
		VkStructureType                  sType;
		const void* pNext;
		VkDescriptorSet                  dstSet;
		uint32_t                         dstBinding;
		uint32_t                         dstArrayElement;
		uint32_t                         descriptorCount;
		VkDescriptorType                 descriptorType;
		const VkDescriptorImageInfo* pImageInfo;
		const VkDescriptorBufferInfo* pBufferInfo;
		const VkBufferView* pTexelBufferView;
	} VkWriteDescriptorSet;
	*/

	writeInfo.dstSet = postprocessDescriptorSet;
	writeInfo.dstBinding = 0;
	writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo.pBufferInfo = &cameraUBODescriptor;

	
	

	vk::WriteDescriptorSet writeInfo2;
	writeInfo2.dstSet = postprocessDescriptorSet;
	writeInfo2.dstBinding = 1;
	writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo2.descriptorCount = 1;
	writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo2.pBufferInfo = &modelsSBODescriptor;

	vk::WriteDescriptorSet writeInfo4;
	writeInfo4.dstSet = particleSBODescriptorSet;
	writeInfo4.dstBinding = 1;
	writeInfo4.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo4.descriptorCount = 1;
	writeInfo4.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo4.pBufferInfo = &ParticleUBODescriptor;
	
	vk::WriteDescriptorSet writeInfo5;
	writeInfo5.dstSet = particleSBODescriptorSet;
	writeInfo5.dstBinding = 2;
	writeInfo5.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo5.descriptorCount = 1;
	writeInfo5.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo5.pBufferInfo = &DeltaTimeDescriptor;

	vk::WriteDescriptorSet writeInfo6;
	writeInfo6.dstSet = particleSBODescriptorSet;
	writeInfo6.dstBinding = 3;
	writeInfo6.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo6.descriptorCount = 1;
	writeInfo6.descriptorType = vk::DescriptorType::eStorageBuffer;


	if (isFirstSSBOParticleActive->load(std::memory_order_relaxed))
		writeInfo6.pBufferInfo = &seccondParticleRandomSSBO->descriptorInfo;
	else 
		writeInfo6.pBufferInfo = &firstParticleRandomSSBO->descriptorInfo;

	logicalDevice.updateDescriptorSets(writeInfo, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo2, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo4, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo5, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo6, nullptr);







	vk::DescriptorImageInfo imageDescriptor;
	imageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptor.imageView = postProcessImageView;
	imageDescriptor.sampler = sampler;

	vk::WriteDescriptorSet writeInfo7;
	writeInfo7.dstSet = fogUBODescriptorSet;
	writeInfo7.dstBinding = 0;
	writeInfo7.dstArrayElement = 0;
	writeInfo7.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	writeInfo7.descriptorCount = 1;
	writeInfo7.pImageInfo = &imageDescriptor;

	vk::WriteDescriptorSet writeInfo8;
	writeInfo8.dstSet = fogUBODescriptorSet;
	writeInfo8.dstBinding = 1;
	writeInfo8.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo8.descriptorCount = 1;
	writeInfo8.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo8.pBufferInfo = &fogUBODescriptor;
	logicalDevice.updateDescriptorSets(writeInfo7, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo8, nullptr);


	if (!isParticleInit) {
		isParticleInit = true;
		vk::WriteDescriptorSet writeInfo3;
		writeInfo3.dstSet = particleSBODescriptorSet;
		writeInfo3.dstBinding = 0;
		writeInfo3.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
		writeInfo3.descriptorCount = 1;
		writeInfo3.descriptorType = vk::DescriptorType::eStorageBuffer;
		writeInfo3.pBufferInfo = &particleDescriptor->descriptorInfo;

		logicalDevice.updateDescriptorSets(writeInfo3, nullptr);
	}
}

void vkUtil::SwapChainFrame::write_UI_descriptors() {

	vk::WriteDescriptorSet writeInfo;

	writeInfo.dstSet = UIDescriptorSet;
	writeInfo.dstBinding = 0;
	writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo.pBufferInfo = &UIPositionSizeDescriptor;


	vk::WriteDescriptorSet writeInfo2;

	writeInfo2.dstSet = UIFontDescriptorSet;
	writeInfo2.dstBinding = 0;
	writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo2.descriptorCount = 1;
	writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo2.pBufferInfo = &UIFontPositionSizeDescriptor;

	logicalDevice.updateDescriptorSets(writeInfo, nullptr);
	logicalDevice.updateDescriptorSets(writeInfo2, nullptr);


}

void vkUtil::SwapChainFrame::destroy(uint32_t index){

	//logicalDevice.destroyImage(mainimage);
	logicalDevice.destroyImage(depthBuffer);
	logicalDevice.destroyImageView(mainimageView);
	logicalDevice.destroyImageView(depthBufferView);

	logicalDevice.freeMemory(depthBufferMemory);

	logicalDevice.unmapMemory(cameraDataBuffer.bufferMemory);
	logicalDevice.freeMemory(cameraDataBuffer.bufferMemory);
	logicalDevice.destroyBuffer(cameraDataBuffer.buffer);

	logicalDevice.unmapMemory(modelsDataBuffer.bufferMemory);
	logicalDevice.freeMemory(modelsDataBuffer.bufferMemory);
	logicalDevice.destroyBuffer(modelsDataBuffer.buffer);

	logicalDevice.unmapMemory(UIPositionSizeDataBuffer.bufferMemory);
	logicalDevice.freeMemory(UIPositionSizeDataBuffer.bufferMemory);
	logicalDevice.destroyBuffer(UIPositionSizeDataBuffer.buffer);

	logicalDevice.unmapMemory(UIFontPositionSizeDataBuffer.bufferMemory);
	logicalDevice.freeMemory(UIFontPositionSizeDataBuffer.bufferMemory);
	logicalDevice.destroyBuffer(UIFontPositionSizeDataBuffer.buffer);


	//logicalDevice.unmapMemory(particleSBODataBuffer.bufferMemory);
	//logicalDevice.freeMemory(particleSBODataBuffer.bufferMemory);
	//logicalDevice.destroyBuffer(particleSBODataBuffer.buffer);

	logicalDevice.destroySemaphore(imageAvailable);
	logicalDevice.destroySemaphore(renderFinished);
	logicalDevice.destroySemaphore(computeFinished);
	logicalDevice.destroyFence(inFlight);
	//logicalDevice.destroyFramebuffer(mainframebuffer);
	if (index ==0) {
		delete particleDescriptor;
	}

	
}
