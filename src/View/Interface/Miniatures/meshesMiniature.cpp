#include "View/Interface/Thumbs/meshesThumbs.h"
#include <View/Interface/Thumbs/thumbRenderer.h>
#include <View/vkInit/descrpitors.h>


vkThumbs::MeshesTumbs::MeshesTumbs(ThumbInput input, MeshesThumbInput meshesInput) : Thumb(input){


	make_descrptor_set(meshesInput);

}

vkThumbs::MeshesTumbs::~MeshesTumbs() {
	for (uint32_t i = 0; i < image.size(); ++i) {
		device.freeMemory(imageMemory[i]);
		device.destroyImage(image[i]);
		device.destroyImageView(imageView[i]);
	}
	device.destroySampler(sampler);
}

ImTextureID vkThumbs::MeshesTumbs::get_texture_icon(int index)
{

	VkDescriptorSet qqq = descriptorsSet[index];
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);


	return imguiTextureId;
}

void vkThumbs::MeshesTumbs::make_descrptor_set(MeshesThumbInput meshesInput) {
	/*struct ThumbRendererInput {
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;
	vkMesh::VertexMenagerie* meshes;
	vk::Format pictureFormat;
	vk::Format depthFormat;
	int width;
	int heigh;
	int number_of_models;
};*/
	
	ThumbRendererInput input;
	input.physicalDevice = physicalDevice;
	input.device = device;
	input.queue = queue;
	input.commandBuffer = commandBuffer;
	input.meshes = meshesInput.meshes;
	input.pictureFormat = meshesInput.pictureFormat;
	input.depthFormat = meshesInput.depthFormat;
	input.models = meshesInput.models;
	input.width = meshesInput.width;
	input.heigh = meshesInput.heigh;
	input.number_of_models = meshesInput.number_of_models;
	int modelsCount = meshesInput.number_of_models;

	ThumbRenderer* renderer = new ThumbRenderer(input, true);
	ThumbRendererOutput images = renderer->get_meshes_images();
	image = images.image;
	imageView = images.imageView;
	imageMemory = images.imageMemory;
	delete renderer;


	for (uint32_t i = 0; i < imageView.size(); ++i) {
		descriptorsSet.push_back(vkInit::allocate_descriptor_set(device, descriptorPool, layout));
	}

	
	
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
	
	for (uint32_t i = 0; i < modelsCount; ++i) {

		vk::DescriptorImageInfo imageDescriptorInfo;
		imageDescriptorInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageDescriptorInfo.imageView = imageView[i];
		imageDescriptorInfo.sampler = sampler;

		vk::WriteDescriptorSet descriptorWrite;
		descriptorWrite.dstSet = descriptorsSet[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageDescriptorInfo;

		device.updateDescriptorSets(descriptorWrite, nullptr);
	}
	
	
}

