#pragma once

namespace UI{
class UICheckbox : public UIElement {
private:
    bool isChecked = false;
    Texture checkTexture; // Tekstura zaznaczenia
public:
    void SetChecked(bool state);
    void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView imageView) override;
    void OnEvent(Event e) override;
};


}