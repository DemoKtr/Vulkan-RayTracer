#pragma once
#include "UI/UIStructures/UIElement.h"
#include "View/vkImage/texture.h"

namespace UI{
class UIText : public UIElement {
private:
    std::string text;
    vkImage::Texture font;
    glm::vec3 color;
public:
    void SetText(const std::string& newText);
    void SetFont(const std::string& fontFilePath) {

    }
    void SetColor(const glm::vec3& newColor);
    glm::vec3& SetColor();
    void Render((vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet dset) override;



};
}