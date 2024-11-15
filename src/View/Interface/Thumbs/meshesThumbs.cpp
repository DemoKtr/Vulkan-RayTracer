#include "View/Interface/Thumbs/meshesThumbs.h"
#include <View/Interface/Thumbs/thumbRenderer.h>
#include <View/vkInit/descrpitors.h>


vkThumbs::MeshesTumbs::MeshesTumbs(ThumbInput input, MeshesThumbInput meshesInput) : Thumb(input){


	make_descrptor_set(meshesInput);

}

vkThumbs::MeshesTumbs::~MeshesTumbs() {
}

ImTextureID vkThumbs::MeshesTumbs::get_texture_icon(int index)
{

	VkDescriptorSet qqq = descriptorsSet[index];
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	std::cout << descriptorsSet.size()<<std::endl;
	std::cout <<"Kurwa" << std::endl;
	std::cout << "Kurwa" <<std::endl;
	std::cout << "Kurwa" <<std::endl;
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
	input.width = meshesInput.width;
	input.heigh = meshesInput.heigh;
	input.number_of_models = meshesInput.number_of_models;
	
	ThumbRenderer* renderer = new ThumbRenderer(input, true);
	ThumbRendererOutput images = renderer->get_meshes_images();
	image = images.image;
	imageView = imageView;
	imageMemory = imageMemory;
	delete renderer;

	

	for (uint32_t i = 0; i < imageView.size(); ++i) {
		descriptorsSet.push_back(vkInit::allocate_descriptor_set(device, descriptorPool, layout));
	}

	std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

	for (uint32_t i = 0; i < descriptorsSet.size(); ++i) {
		vk::DescriptorImageInfo imageDescriptorInfo;
		imageDescriptorInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageDescriptorInfo.imageView = imageView[i];
		imageDescriptorInfo.sampler = VK_NULL_HANDLE;

		vk::WriteDescriptorSet descriptorWritePos;
		descriptorWritePos.dstSet = descriptorsSet[i];
		descriptorWritePos.dstBinding = 0;
		descriptorWritePos.dstArrayElement = 0;
		descriptorWritePos.descriptorType = vk::DescriptorType::eInputAttachment;
		descriptorWritePos.descriptorCount = 1;
		descriptorWritePos.pImageInfo = &imageDescriptorInfo;

		writeDescriptorSets.push_back(descriptorWritePos);
		
	}
	device.updateDescriptorSets(writeDescriptorSets, nullptr);
	
}

