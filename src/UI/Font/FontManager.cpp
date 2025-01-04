#include "UI/Font/FontManager.h"

UI::FontManager::FontManager(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool, vk::CommandBuffer commandBuffer) {
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	std::vector<unsigned char*> glyphTextureData;
	for (std::string path : filesManager.getFontNames().fullPaths) {
		std::map<char, fileOperations::GlyphInfo> glyphMap;
		glyphTextureData.push_back(fileOperations::generateSDFAtlas(path, 1024, 48, glyphMap).data());
		uint64_t hash = filesManager.getFontNames().hash[path];
		texturesUV[hash] = glyphMap;

	}
	vkImage::TextureDataInputChunk input;
	input.logicalDevice = device;
	input.physicalDevice = physicalDevice;
	input.commandBuffer = commandBuffer;
	input.queue = queue;
	input.descriptorPool = descriptorPool;
	input.layout = layout;
	input.width = 1024;
	input.height = 1024;
	input.data = glyphTextureData;
	texture = new vkImage::Texture(input);
}
