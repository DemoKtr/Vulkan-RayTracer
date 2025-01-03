#pragma once

namespace UI{
class UITooltip : public UIElement {
private:
    std::string content;
    UIText textElement; // Tooltip może używać `UIText`
public:
    void SetContent(const std::string& text);
    void Render() override;
};


}