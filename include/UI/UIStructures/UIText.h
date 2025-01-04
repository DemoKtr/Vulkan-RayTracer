#pragma once
#include "UI/UIStructures/UIElement.h"
#include "View/vkImage/texture.h"
#include "View/vkUtil/frame.h"
#include "fileOperations/fontLoader.h"

namespace UI{
class UIText : public UIElement {
private:
    std::string text = "ABCD";
    glm::vec3 color;
    uint64_t font;
public:
    std::string& GetText();
    void SetFont(const uint64_t font);
    uint64_t GetFont();
    void SetColor(const glm::vec3& newColor);
    glm::vec3& GetColor();
    
    void Update(vkUtil::FontSBO sbo,std::map<char,fileOperations::GlyphInfo>& glyphMap);


};
}