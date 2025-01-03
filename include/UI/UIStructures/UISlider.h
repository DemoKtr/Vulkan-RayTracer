#pragma once

namespace UI{
class UISlider : public UIElement {
private:
    float value = 0.5f;       // Wartość suwaka
    Texture trackTexture;     // Tekstura ścieżki suwaka
    Texture knobTexture;      // Tekstura gałki suwaka

public:
    void SetValue(float newValue) { value = std::clamp(newValue, 0.0f, 1.0f); }
    void SetTrackTexture(Texture texture) { trackTexture = texture; }
    void SetKnobTexture(Texture texture) { knobTexture = texture; }

    void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView imageView) override {
      
    }

    void OnEvent(Event e) override {
        // Obsługa przesuwania gałki (np. w zależności od pozycji myszy)
        if (e.type == EventType::MouseDrag) {
            float newValue = (e.mousePosition.x - position.x) / size.x;
            SetValue(newValue);
        }
    }
};

}