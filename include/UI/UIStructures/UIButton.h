#pragma once
#include "UI/UIStructures/UIElement.h"
#include "View/vkInit/vkPipeline/pipelineCache.h"
#include <View/vkResources/resources.h>
#include "settings.h"

namespace UI{
    class UIButton : public UIElement {
    private:
        uint64_t normalTexture;  // Tekstura przycisku w stanie normalnym
        uint64_t hoverTexture;   // Tekstura przycisku w stanie najechania
        uint64_t pressedTexture; // Tekstura przycisku w stanie kliknięcia
        bool isHovered = false; // Czy kursor znajduje się nad przyciskiem
        bool isPressed = false; // Czy przycisk jest kliknięty

        

    public:
        UIButton(glm::vec2 pos, glm::vec2 size);

        // Ustawianie tekstur
        void SetNormalTexture(uint64_t texture) { normalTexture = texture; }
        void SetHoverTexture(uint64_t texture) { hoverTexture = texture; }
        void SetPressedTexture(uint64_t texture) { pressedTexture = texture; }




        void Update(std::vector<glm::vec4>& PosSize, size_t index);
        // Obsługa zdarzeń
        /*
        void OnEvent(Event e) override {
            // Logikę obsługi przenieśliśmy do callbacków GLFW
        }
        */
      
    };

}