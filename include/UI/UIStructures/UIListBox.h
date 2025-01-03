#pragma once

namespace UI{
class UIListBox : public UIPanel {
private:
    int selectedItemIndex = -1;
public:
    void SetSelectedItem(int index);
    void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView imageView) override;
};

}