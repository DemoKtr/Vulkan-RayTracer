#include "View/Interface/Thumbs/thumbsManager.h"
#include <View/vkInit/descrpitors.h>
#include "settings.h"
void vkThumbs::ThumbsManager::make_descriptor_layout() {
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 1;
	bindings.indices.push_back(0);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);
	descriptorLayout = vkInit::make_descriptor_set_layout(logicalDevice, bindings);
	descriptorPool = vkInit::make_descriptor_pool(logicalDevice, 10, bindings);

 
}



vkThumbs::ThumbsManager::ThumbsManager(ThumbsManagerInput info) {
   
    
    /*
    vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;
    */
    logicalDevice = info.logicalDevice;
    make_descriptor_layout();
   

    ThumbInput input;
    input.physicalDevice = info.physicalDevice;
    input.logicalDevice = info.logicalDevice;
    input.commandBuffer = info.commandBuffer;
    input.queue = info.queue;
    input.layout = descriptorLayout;
    input.descriptorPool = descriptorPool;


    /*
    vkMesh::VertexMenagerie* meshes;
		vk::Format pictureFormat;
		vk::Format depthFormat;
		int number_of_models;
		int width;
		int heigh;
    */

    MeshesThumbInput meshesInput;
    meshesInput.meshes = info.meshes;
    meshesInput.pictureFormat = info.pictureFormat;
    meshesInput.depthFormat = info.depthFormat;
    meshesInput.number_of_models = info.number_of_models;
    meshesInput.heigh = vkSettings::thumbHeigh;
    meshesInput.width = vkSettings::thumbWidth;

    iconsThums = new IconThumbs(input);
    meshesThumbs = new MeshesTumbs(input,meshesInput);
    texturesThumbs = new TextureThumbs(input,info.textures);

}

vkThumbs::ThumbsManager::~ThumbsManager() {
    logicalDevice.destroyDescriptorPool(descriptorPool);
    logicalDevice.destroyDescriptorSetLayout(descriptorLayout);
}

ImTextureID vkThumbs::ThumbsManager::get_folder_icon() {
    return iconsThums->get_folder_icon();
}

ImTextureID vkThumbs::ThumbsManager::get_header_icon() {
    return iconsThums->get_header_icon();
}

ImTextureID vkThumbs::ThumbsManager::get_source_icon() {
    return iconsThums->get_source_icon();
}

ImTextureID vkThumbs::ThumbsManager::get_text_icon() {
    return iconsThums->get_text_icon();
}

ImTextureID vkThumbs::ThumbsManager::get_file_icon() {
    return iconsThums->get_file_icon();
}

ImTextureID vkThumbs::ThumbsManager::get_mesh_icon(int index){
    return meshesThumbs->get_texture_icon(index);
}

ImTextureID vkThumbs::ThumbsManager::get_texture_icon(int index) {
    return texturesThumbs->get_texture_icon(index);
}
