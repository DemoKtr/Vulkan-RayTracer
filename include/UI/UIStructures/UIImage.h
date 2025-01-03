#pragma once

namespace UI{
class UIImage : public UIElement {
private:
    Texture texture; // Tekstura obrazu

public:
    void SetTexture(Texture newTexture) { texture = newTexture; }

    void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView imageView) override {
      
    }
};

}