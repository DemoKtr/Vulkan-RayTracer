#include "UI/UIStructures/UIText.h"
#include <fileOperations/fontLoader.h>


std::string& UI::UIText::GetText() {
	return text;
}

void UI::UIText::SetFont(const uint64_t font) {
	this->font = font;
}

uint64_t UI::UIText::GetFont() {
	return font;
}

void UI::UIText::SetColor(const glm::vec3& newColor) {
	this->color = newColor;
}

glm::vec3& UI::UIText::GetColor() {
	return color;
}

void UI::UIText::Update(vkUtil::FontSBO sbo, std::map<char, fileOperations::GlyphInfo>& glyphMap) {
     // Wierzcho�ki (pozycja + UV)
    float penX = this->position.x;
    float penY = this->position.y;
    for (const char& c : text) {
        
        fileOperations::GlyphInfo glyph = glyphMap[c];

        float x0 = penX + glyph.offsetX;              // Pozycja X lewego g�rnego wierzcho�ka
        float y0 = penY - glyph.offsetY;              // Pozycja Y lewego g�rnego wierzcho�ka
        float x1 = x0 + glyph.width;                  // Pozycja X prawego dolnego wierzcho�ka
        float y1 = y0 - glyph.height;                 // Pozycja Y prawego dolnego wierzcho�ka

        float uvLeft = glyph.x;                       // UV lewego boku
        float uvTop = glyph.y;                        // UV g�rnego boku
        float uvRight = glyph.x + glyph.width;        // UV prawego boku
        float uvBottom = glyph.y + glyph.height;      // UV dolnego boku
        /*
        // Dodawanie prostok�ta (quad) do danych wierzcho�k�w
        vertexData.insert(vertexData.end(), {
            x0, y0, uvLeft, uvTop,    // LT
            x1, y0, uvRight, uvTop,   // RT
            x1, y1, uvRight, uvBottom,// RB
            x0, y1, uvLeft, uvBottom  // LB
            });

        // Przesuni�cie "pi�ra" do nast�pnego znaku
        penX += glyph.advanceX;
        */
    }
}


