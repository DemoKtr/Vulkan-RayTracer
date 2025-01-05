#pragma once
#include "UI/UIStructures/UIElement.h"
#include "View/vkImage/texture.h"
#include "View/vkUtil/frame.h"
#include "fileOperations/fontLoader.h"
#include <UI/Font/FontManager.h>

namespace UI{
class UIText : public UIElement {
private:
    std::string text = "I Love Vulkan <3";
    glm::vec3 color;
    uint64_t font;

public:
    UIText(glm::vec2 pos, glm::vec2 size);
    std::string& GetText();
    void SetFont(const uint64_t font);
    uint64_t GetFont();
    void SetColor(const glm::vec3& newColor);
    glm::vec3& GetColor();
    
    void Update(std::vector<vkUtil::FontSBO>& sbo, FontManager* fontManager, size_t& lettterOffset);


};
}