#include "View/vkUtil/frame.h"
#include <View/vkUtil/memory.h>
#include <View/vkImage/image.h>

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

	int sbo_size = number_of_objects+2048;
	input.size = sbo_size * sizeof(MeshSBO);
	input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
	modelsDataBuffer = createBuffer(input);
	modelsDataWriteLocation = logicalDevice.mapMemory(modelsDataBuffer.bufferMemory,0, sbo_size *sizeof(MeshSBO));


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

	modelsSBODescriptor.buffer = modelsDataBuffer.buffer;
	modelsSBODescriptor.offset = 0;
	modelsSBODescriptor.range = sbo_size *sizeof(MeshSBO);

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

	logicalDevice.updateDescriptorSets(writeInfo, nullptr);

	vk::WriteDescriptorSet writeInfo2;
	writeInfo2.dstSet = postprocessDescriptorSet;
	writeInfo2.dstBinding = 1;
	writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo2.descriptorCount = 1;
	writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo2.pBufferInfo = &modelsSBODescriptor;

	logicalDevice.updateDescriptorSets(writeInfo2, nullptr);
}

void vkUtil::SwapChainFrame::destroy(){

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


	logicalDevice.destroySemaphore(imageAvailable);
	logicalDevice.destroySemaphore(renderFinished);
	logicalDevice.destroySemaphore(computeFinished);
	logicalDevice.destroyFence(inFlight);
	//logicalDevice.destroyFramebuffer(mainframebuffer);


}
