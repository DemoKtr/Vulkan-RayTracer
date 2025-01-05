#include "UI/Font/FontManager.h"
#include <iostream>
UI::FontManager::FontManager(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool, vk::CommandBuffer commandBuffer) {
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	std::vector<unsigned char*> glyphTextureData;
	for (std::string path : filesManager.getFontNames().fullPaths) {
		std::map<char, fileOperations::GlyphInfo> glyphMap;
		std::vector<unsigned char> atlas = fileOperations::generateSDFAtlas(path, 1024, 48, glyphMap);
		unsigned char* atlasCopy = new unsigned char[atlas.size()];
		std::copy(atlas.begin(), atlas.end(), atlasCopy);

		// Dodaj wskaŸnik na skopiowane dane do glyphTextureData
		glyphTextureData.push_back(atlasCopy);
		uint64_t hash = filesManager.getFontNames().hash[path];
		texturesUV[hash] = glyphMap;
		//this->printGlyphMap(glyphMap);
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

void UI::FontManager::printGlyphMap(const std::map<char, fileOperations::GlyphInfo>& glyphMap)
{
	for (const auto& entry : glyphMap) {
		const char& character = entry.first;
		const fileOperations::GlyphInfo& glyph = entry.second;

		std::cout << "Character: '" << character << "'\n";
		std::cout << "  x: " << glyph.x << "\n";
		std::cout << "  y: " << glyph.y << "\n";
		std::cout << "  width: " << glyph.width << "\n";
		std::cout << "  height: " << glyph.height << "\n";
		std::cout << "  advanceX: " << glyph.advanceX << "\n";
		std::cout << "  offsetX: " << glyph.offsetX << "\n";
		std::cout << "  offsetY: " << glyph.offsetY << "\n";
		std::cout << "--------------------------\n";
	}
}
