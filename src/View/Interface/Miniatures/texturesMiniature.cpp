#include "View/Interface/Thumbs/texturesThumbs.h"
#include "fileOperations/resources.h"
vkThumbs::TextureThumbs::TextureThumbs(ThumbInput input) : Thumb(input) {

	/*struct TextureInputChunk {
			vk::Device logicalDevice;
			vk::PhysicalDevice physicalDevice;
			TexturesNames texturesNames;
			const char* filenames;
			vk::CommandBuffer commandBuffer;
			vk::Queue queue;
			vk::DescriptorSetLayout layout;
			vk::DescriptorPool descriptorPool;

		};*/
	vkImage::TextureInputChunk info;
	info.logicalDevice = device;
	info.physicalDevice = physicalDevice;
	info.commandBuffer = commandBuffer;
	info.queue = queue;
	info.layout = layout;
	info.descriptorPool = descriptorPool;


	info.filenames = nullptr;

	for (std::string filename : fileOperations::texturesNames.fullPaths) {
		info.filenames = filename.c_str();
		vkImage::Texture* icon = new vkImage::Texture(info);
		this->textures.push_back(icon);
	}

}

vkThumbs::TextureThumbs::~TextureThumbs() {
	for (vkImage::Texture* texture : this->textures) {
		delete texture;
	}
}



ImTextureID vkThumbs::TextureThumbs::get_texture_icon(int index){
	VkDescriptorSet qqq = textures[index]->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}
