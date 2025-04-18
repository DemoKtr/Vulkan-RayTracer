#pragma once
#include "UI/UIManager.h"
#include "UI/UIStructures/UIElements.h"
#include "UI/UIInput.h"
namespace UI {
	class UIManager {
	private:
		UIElementManager<UIButton> buttonManager;
		UIElementManager<UIText> textManager;

	public:
        //256
		UIManager() : buttonManager(128), textManager(128)  {}


		UIButton* create_button(glm::vec2 pos, glm::vec2 size) {
			return buttonManager.createElement(pos,size);
		}
		void remove_button(UIButton* button) {
			buttonManager.removeElement(button);
		}

        UIText* create_text(glm::vec2 pos, glm::vec2 size) {
            return textManager.createElement(pos, size);
        }
        void remove_text(UIText* text) {
            textManager.removeElement(text);
        }

		void render_ui(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet& dset, vk::DescriptorSet& fontdset, UI::UIRenderingDrawData renderingDrawData,FontManager* fontManager) {
            vk::RenderingAttachmentInfoKHR colorAttachment = {};
            colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfoKHR;
            colorAttachment.imageView = imageView; // Widok obrazu.
            colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
            colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
            colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;


            vk::RenderingInfoKHR renderingInfo = {};
            renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
            renderingInfo.renderArea.extent.width = swapchainExtent.width;
            renderingInfo.renderArea.extent.height = swapchainExtent.height;
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachment;
            renderingInfo.pDepthAttachment = nullptr;

            glm::vec2 screenSize = glm::vec2(swapchainExtent.width, swapchainExtent.height);
            vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("UI Pipeline");
           

            // Rozpoczęcie renderowania
            try {
                commandBuffer.beginRendering(&renderingInfo);

            }
            catch (vk::SystemError err) {
                std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
                return;
            }

            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, dset, nullptr);
            commandBuffer.pushConstants(pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::vec2), &screenSize);
            commandBuffer.draw(3,renderingDrawData.UIinstanceCount , 0, 0);


            pipelineInfo = vkResources::scenePipelines->getPipeline("UIFont Pipeline");
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, fontdset, nullptr);
            commandBuffer.pushConstants(pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::vec2), &screenSize);
            fontManager->BindFontTexture(commandBuffer, pipelineInfo.pipelineLayout,1);
            commandBuffer.draw(3, renderingDrawData.UILettersinstanceCount, 0, 0);
            
            commandBuffer.endRendering();
           


		}

		void update(std::vector<glm::vec4>& PosSize,UI::UIRenderingDrawData& renderingDrawData,std::vector<vkUtil::FontSBO>& fontSBO, FontManager* fontManager) {
			buttonManager.update(PosSize, renderingDrawData.UIinstanceCount);
			textManager.update(fontSBO, fontManager,renderingDrawData.UILettersinstanceCount);
		}
	};
}
