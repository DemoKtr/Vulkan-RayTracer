#pragma once
#include "UI/UIStructures/UIElement.h"
#include "View/vkInit/vkPipeline/pipelineCache.h"
#include <View/vkResources/resources.h>

namespace UI{
    class UIButton : public UIElement {
    private:
        uint64_t normalTexture;  // Tekstura przycisku w stanie normalnym
        uint64_t hoverTexture;   // Tekstura przycisku w stanie najechania
        uint64_t pressedTexture; // Tekstura przycisku w stanie kliknięcia
        bool isHovered = false; // Czy kursor znajduje się nad przyciskiem
        bool isPressed = false; // Czy przycisk jest kliknięty

        // Callbacki GLFW
        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    public:
        UIButton(glm::vec2 pos, glm::vec2 size) {
            this->position = pos;
            this->size = size;
        }

        // Ustawianie tekstur
        void SetNormalTexture(uint64_t texture) { normalTexture = texture; }
        void SetHoverTexture(uint64_t texture) { hoverTexture = texture; }
        void SetPressedTexture(uint64_t texture) { pressedTexture = texture; }

        // Renderowanie przycisku
        void Render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView,vk::DescriptorSet dset) override {

            vk::RenderingAttachmentInfoKHR colorAttachment = {};
            colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfoKHR;
            colorAttachment.imageView = imageView; // Widok obrazu.
            colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
            colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
            colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
            //colorAttachment.clearValue.color = vk::ClearColorValue(color);




            vk::RenderingInfoKHR renderingInfo = {};
            renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
            renderingInfo.renderArea.extent.width = swapchainExtent.width;
            renderingInfo.renderArea.extent.height = swapchainExtent.height;
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachment;
            renderingInfo.pDepthAttachment = nullptr;


            vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("UI Pipeline");

            // Rozpoczęcie renderowania
            try {
                commandBuffer.beginRendering(&renderingInfo);

            }
            catch (vk::SystemError err) {
                std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
                return;
            }


            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, dset, nullptr);
            
            commandBuffer.draw(3, 1, 0, 0);
   
            commandBuffer.endRendering();
        }



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
        // Rejestracja callbacków GLFW
        static void RegisterCallbacks(GLFWwindow* window) {
            glfwSetCursorPosCallback(window, CursorPositionCallback);
            glfwSetMouseButtonCallback(window, MouseButtonCallback);
        }

        // Sprawdzenie, czy kursor jest nad przyciskiem
        bool IsMouseOver(double mouseX, double mouseY) const {
            return mouseX >= position.x && mouseX <= position.x + size.x &&
                mouseY >= position.y && mouseY <= position.y + size.y;
        }
    };

}