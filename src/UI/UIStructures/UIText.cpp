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
     // Wierzchołki (pozycja + UV)
    float penX = this->position.x;
    float penY = this->position.y;
    for (const char& c : text) {
        
        fileOperations::GlyphInfo glyph = glyphMap[c];

        float x0 = penX + glyph.offsetX;              // Pozycja X lewego górnego wierzchołka
        float y0 = penY - glyph.offsetY;              // Pozycja Y lewego górnego wierzchołka
        float x1 = x0 + glyph.width;                  // Pozycja X prawego dolnego wierzchołka
        float y1 = y0 - glyph.height;                 // Pozycja Y prawego dolnego wierzchołka

        float uvLeft = glyph.x;                       // UV lewego boku
        float uvTop = glyph.y;                        // UV górnego boku
        float uvRight = glyph.x + glyph.width;        // UV prawego boku
        float uvBottom = glyph.y + glyph.height;      // UV dolnego boku
        /*
        // Dodawanie prostokąta (quad) do danych wierzchołków
        vertexData.insert(vertexData.end(), {
            x0, y0, uvLeft, uvTop,    // LT
            x1, y0, uvRight, uvTop,   // RT
            x1, y1, uvRight, uvBottom,// RB
            x0, y1, uvLeft, uvBottom  // LB
            });

        // Przesunięcie "pióra" do następnego znaku
        penX += glyph.advanceX;
        */
    }
}


