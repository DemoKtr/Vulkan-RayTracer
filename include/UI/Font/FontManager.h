#pragma once
#include "View/vkImage/texture.h"
#include "fileOperations/fontLoader.h"
#include "fileOperations/resources.h"
namespace UI {

	class FontManager {
		
		std::unordered_map<uint64_t, std::map<char,fileOperations::GlyphInfo>> texturesUV;
		vkImage::Texture* texture;
	public:
		FontManager(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool, vk::CommandBuffer commandBuffer); 


		void BindFontTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, int layoutIndex) {
			this->texture->useTexture(commandBuffer,pipelineLayout,layoutIndex);
		}

		std::map<char, fileOperations::GlyphInfo>& GetFontData(uint64_t fontId) {

			return texturesUV[fontId];
		}

		~FontManager() {
			delete texture;
		}


	};

}