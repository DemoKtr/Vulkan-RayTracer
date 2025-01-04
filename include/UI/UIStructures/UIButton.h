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
        UIButton(glm::vec2 pos, glm::vec2 size) {
            this->position = pos;
            this->size = size;
        }

        // Ustawianie tekstur
        void SetNormalTexture(uint64_t texture) { normalTexture = texture; }
        void SetHoverTexture(uint64_t texture) { hoverTexture = texture; }
        void SetPressedTexture(uint64_t texture) { pressedTexture = texture; }




        void Update(std::vector<glm::vec4>& PosSize, size_t index) {
            float x = this->position.x;///1920.0f *2 -1;
            float y = this->position.y;///1080.0f *2 -1;
            float z = this->size.x;///1920.0f *2;
            float w = this->size.y;///1080.0f *2;
            PosSize[index]=(glm::vec4(x,y,z,w));
        }
        // Obsługa zdarzeń
        /*
        void OnEvent(Event e) override {
            // Logikę obsługi przenieśliśmy do callbacków GLFW
        }
        */
      
    };

}