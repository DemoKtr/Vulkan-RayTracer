#pragma once
#include "config.h"

namespace UI{
class UIElement {
protected:
    glm::vec2 position;      // Pozycja elementu
    glm::vec2 size;          // Rozmiar elementu
    bool visible = true;   // Widoczność elementu
    bool interactive = false; // Czy element może reagować na zdarzenia

public:
   
    
    // Metody wspólne dla wszystkich elementów
    void SetPosition(glm::vec2 newPosition) { position = newPosition; }
    void SetSize(glm::vec2 newSize) { size = newSize; }
    void SetVisibility(bool isVisible) { visible = isVisible; }
    glm::vec2& GetPosition() { return position; }
    glm::vec2& GetSize() { return size; }
    bool GetVisibility() { return visible; }
    bool GetInteractive() { return  interactive; }
    
};
}