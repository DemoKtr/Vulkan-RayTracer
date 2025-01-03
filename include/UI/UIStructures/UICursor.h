#pragma once
#include "UI/UIStructures/UIElement.h"
namespace UI{
    class UICursor : public UIElement {
       // Texture texture
    public:
        void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet dset) override;
       // void SetTexture(Texture newTexture);
    };


}