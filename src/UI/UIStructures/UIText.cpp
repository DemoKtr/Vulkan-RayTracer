#include "UI/UIStructures/UIText.h"
#include <fileOperations/fontLoader.h>


UI::UIText::UIText(glm::vec2 pos, glm::vec2 size) {
    fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
    font = filesManager.getFontNames().hash[filesManager.getFontNames().fullPaths[0]];
    this->position = pos;
    this->size = size;
}

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

void UI::UIText::Update(std::vector<vkUtil::FontSBO>& sbo, FontManager* fontManager, size_t& lettterOffset) {
    float penX = 0;
    float penY = 0;

    std::map<char, fileOperations::GlyphInfo> glyphMap = fontManager->GetFontData(font);

    float atlasWidth = 1024.0f;
    float atlasHeight = 1024.0f;

    for (char& c : text) {
        if (glyphMap.find(c) == glyphMap.end()) {
            // Obs�uga brakuj�cych znak�w
            penX += glyphMap[' '].advanceX;  // Mo�na u�y� szeroko�ci spacji
            continue;
        }

        fileOperations::GlyphInfo glyph = glyphMap[c];

        // Oblicz rozmiary w pikselach
        float glyphPixelWidth = glyph.width * atlasWidth;
        float glyphPixelHeight = glyph.height * atlasHeight;

        // Wype�nianie FontSBO
        vkUtil::FontSBO fontSBO;
        fontSBO.PosSize = glm::vec4(
            this->position.x + penX + glyph.offsetX,  // Pozycja X z uwzgl�dnieniem offsetu
            this->position.y + penY - glyph.offsetY,  // Pozycja Y (odwr�� dla FreeType)
            glyphPixelWidth * 1,
            glyphPixelHeight * 1
        );
        fontSBO.UVBounds = glm::vec4(
            glyph.x,
            glyph.y,
            glyph.x + glyph.width,
            glyph.y + glyph.height
        );
        fontSBO.textures = glm::uvec4(0, 0, 0, 0);  // Dostosuj, je�li u�ywasz wielu atlas�w

        sbo[lettterOffset++] = fontSBO;

        // Przesuni�cie "pi�ra"
        penX += glyph.advanceX;

        // Obs�uga �amania linii
        if (penX > this->size.x) {
            penX = 0;
            penY += this->size.y;
        }
    }
}


