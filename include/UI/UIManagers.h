#pragma once
#include "UI/UIManager.h"
#include "UI/UIStructures/UIElements.h"
namespace UI {
	class UIManager {
	private:
		UIElementManager<UIButton> buttonManager;

	public:
		UIManager() : buttonManager(128) {}


		UIButton* create_button(glm::vec2 pos, glm::vec2 size) {
			return buttonManager.createElement(pos,size);
		}
		void remove_button(UIButton* button) {
			buttonManager.removeElement(button);
		}

		void render_ui(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet dset) {
			buttonManager.renderAll( commandBuffer,  swapchainExtent, imageView,dset);


		}

		void update(std::vector<glm::vec4>& PosSize,size_t& count) {
			buttonManager.updateAll(PosSize, count);
		}
	};
}
