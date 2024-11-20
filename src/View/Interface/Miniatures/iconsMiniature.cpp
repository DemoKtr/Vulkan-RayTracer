#include "View/Interface/Thumbs/iconsThumbs.h"


vkThumbs::IconThumbs::IconThumbs(ThumbInput input) : Thumb(input) {



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
	info.filenames = "resources/textures/folder.jpg";
	info.commandBuffer = commandBuffer;
	info.queue = queue;
	info.layout = layout;
	info.descriptorPool = descriptorPool; 

	folderIcon = new vkImage::Texture(info);
	std::cout << "test2" << std::endl;
	info.filenames = "resources/textures/header.jpg";
	headerIcon = new vkImage::Texture(info);
	std::cout << "test1" << std::endl;
	info.filenames = "resources/textures/source.jpg";
	sourceIcon = new vkImage::Texture(info);
	std::cout << "test3" << std::endl;
	info.filenames = "resources/textures/text.jpg";
	textIcon = new vkImage::Texture(info);
	std::cout << "test4" << std::endl;
	info.filenames = "resources/textures/file.jpg";
	fileIcon = new vkImage::Texture(info);
	std::cout << "test5" << std::endl;
	
}

vkThumbs::IconThumbs::~IconThumbs() {
	delete folderIcon;
	delete headerIcon;
	delete sourceIcon;
	delete textIcon;
	delete fileIcon;
}

ImTextureID vkThumbs::IconThumbs::get_folder_icon() {
	VkDescriptorSet qqq = folderIcon->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}

ImTextureID vkThumbs::IconThumbs::get_header_icon() {

	VkDescriptorSet qqq = headerIcon->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}

ImTextureID vkThumbs::IconThumbs::get_source_icon() {

	VkDescriptorSet qqq = sourceIcon->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}

ImTextureID vkThumbs::IconThumbs::get_text_icon() {

	VkDescriptorSet qqq = textIcon->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}

ImTextureID vkThumbs::IconThumbs::get_file_icon() {

	VkDescriptorSet qqq = fileIcon->getDescriptorSet();
	ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
	return imguiTextureId;
}
