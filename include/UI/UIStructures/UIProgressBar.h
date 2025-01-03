#pragma once

namespace UI{
class UIProgressBar : public UIElement {
private:
    float progress = 0.0f;       // Wartość od 0.0 do 1.0
    Texture borderTexture;       // Tekstura obramowania
    Texture fillTexture;         // Tekstura wypełnienia

public:
    void SetProgress(float value) { progress = std::clamp(value, 0.0f, 1.0f); }
    void SetBorderTexture(Texture newTexture) { borderTexture = newTexture; }
    void SetFillTexture(Texture newTexture) { fillTexture = newTexture; }
    
    void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView imageView) override {

    }
};

}