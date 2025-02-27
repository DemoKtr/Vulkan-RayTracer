#include "View/vkPostProcess/Fog.h"
#include <View/vkInit/vkPipeline/pipelineCache.h>
#include <View/vkResources/resources.h>


void postprocess::Fog::render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet& dset) {
    std::array<float, 4> color = { 0.0f,0.0f, 0.0f, 1.0f };
    
    vk::RenderingAttachmentInfoKHR colorAttachment = {};
    colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfoKHR;
    colorAttachment.imageView = imageView; // Widok obrazu.
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue.color = vk::ClearColorValue(color);

    vk::RenderingInfoKHR renderingInfo = {};
    renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
    renderingInfo.renderArea.extent.width = swapchainExtent.width;
    renderingInfo.renderArea.extent.height = swapchainExtent.height;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = nullptr;

    glm::vec2 screenSize = glm::vec2(swapchainExtent.width, swapchainExtent.height);
    vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("Fog");


    // Rozpoczêcie renderowania
    try {
        commandBuffer.beginRendering(&renderingInfo);

    }
    catch (vk::SystemError err) {
        std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
        return;
    }

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, dset, nullptr);
    texture->useTexture(commandBuffer, pipelineInfo.pipelineLayout,1);
    
    commandBuffer.draw(6, 1, 0, 0);




    commandBuffer.endRendering();

}

void postprocess::Fog::generateData(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::CommandBuffer commandBuffer, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool) {
    float scale = 0.004f;
    int seed = 432;
    int octaves = 8;
    float persistence = 0.5f; // kontroluje malej¹c¹ amplitudê kolejnych oktaw

    std::vector<float> data(width * height * depth);

    for (int z = 0; z < depth; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float noise = 0.0f;
                float amplitude = 1.0f;
                float frequency = 1.0f;

                for (int i = 0; i < octaves; ++i) {
                    // Przesuniêcie wspó³rzêdnych o 0.5f zapobiega artefaktom symetrii
                    float fx = (x + 0.8f + seed) * scale * frequency;
                    float fy = (y + 0.8f + seed) * scale * frequency;
                    float fz = (z + 0.8f + seed) * scale * frequency;

                    noise += amplitude * stb_perlin_noise3(fx, fy, fz, 1024, 1024, 1024);

                    amplitude *= persistence;
                    frequency *= 2.0f;
                }

                // Normalizacja wartoœci do zakresu [0,1]
                noise = (noise + 1.0f) * 0.5f;
                data[x + y * width + z * width * height] = noise * 0.8f;
            }
        }
    }



    vkImage::Texture3DInput input;
    input.physicalDevice = physicalDevice;
    input.logicalDevice = device;
    input.commandBuffer = commandBuffer;
    input.queue = queue;
    input.layout = layout;
    input.descriptorPool = descriptorPool;
    input.width = width;
    input.height = height;
    input.depth = depth;
    input.data = data;

    texture = new vkImage::Texture3D(input);
}

