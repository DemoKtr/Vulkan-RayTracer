#include "UI/UIStructures/UIButton.h"

UI::UIButton::UIButton(glm::vec2 pos, glm::vec2 size) {
    this->position = pos;
    this->size = size;
}

void UI::UIButton::Update(std::vector<glm::vec4>& PosSize, size_t index) {
    float x = this->position.x;///1920.0f *2 -1;
    float y = this->position.y;///1080.0f *2 -1;
    float z = this->size.x;///1920.0f *2;
    float w = this->size.y;///1080.0f *2;
    PosSize[index] = (glm::vec4(x, y, z, w));
}
