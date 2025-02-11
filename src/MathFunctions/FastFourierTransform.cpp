#include "MathFunctions/FastFourierTransform.h"
#include "cmath"
#include <iostream>
#include <View/vkInit/vkPipeline/pipelineCache.h>
#include <View/vkResources/resources.h>

math::FastFourierTransform::FastFourierTransform(int N, float L_patch, float wind_speed, glm::vec2 wind_dir, float A, float g) : N(N), L_patch(L_patch), wind_speed(wind_speed), A(A), g(g), wind_dir(wind_dir), h0(N* N), omega(N* N), heights(N* N) {

        generateInitialSpectrum();
        vp = vkRenderStructs::getViewProjectionMatrix(1920,1080);

}

math::FastFourierTransform::~FastFourierTransform() {
    delete heighMapTexture;
}

void math::FastFourierTransform::createTexture(vk::PhysicalDevice physicalDevice, vk::Device device, vk::CommandBuffer commandBuffer, vk::Queue queue, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool)
{
    vkImage::TextureFloatDataInputChunk info;
    info.physicalDevice = physicalDevice;
    info.logicalDevice = device;
    info.commandBuffer = commandBuffer;
    info.height = N;
    info.width = N;
    info.queue = queue;
    info.layout = layout;
    info.descriptorPool = descriptorPool;
    info.data = heights;

    heighMapTexture = new vkImage::Texture(info);
}

void math::FastFourierTransform::update(float t)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int idx = i * N + j;
            std::complex<float> h_k = h0[idx] * std::exp(std::complex<float>(0, omega[idx] * t)) +
                std::conj(h0[idx]) * std::exp(std::complex<float>(0, -omega[idx] * t));
            heights[idx] = h_k.real();
        }
    }
}

void math::FastFourierTransform::Render(vkUtil::SwapChainFrame& frame, vk::CommandBuffer& commandBuffer, vk::Extent2D& swapchainExtent, vk::DispatchLoaderDynamic& dldi) {
    vk::RenderingAttachmentInfoKHR colorAttachment = {};
    colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
    colorAttachment.imageView = frame.mainimageView; // Widok obrazu.
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;




    vk::RenderingAttachmentInfoKHR depthAttachment = {};
    depthAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
    depthAttachment.imageView = frame.depthBufferView; // Widok obrazu dla g³êbi.
    depthAttachment.imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eStore;

    // Debugging attachmentów g³êbi


    vk::RenderingInfoKHR renderingInfo = {};
    renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
    renderingInfo.renderArea.extent.width = swapchainExtent.width;
    renderingInfo.renderArea.extent.height = swapchainExtent.height;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;
    vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("Water Mesh Pipeline");
    try {
        commandBuffer.beginRendering(&renderingInfo);

    }
    catch (vk::SystemError err) {
        std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
        return;
    }

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
    heighMapTexture->useTexture(commandBuffer, pipelineInfo.pipelineLayout,0);
    commandBuffer.pushConstants
    (pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eMeshEXT, 0, sizeof(vkRenderStructs::ViewProjectionData), &vp);

    commandBuffer.drawMeshTasksEXT(1, 1, 1, dldi);



    commandBuffer.endRendering();
}

const std::vector<float>& math::FastFourierTransform::getHeights() const
{
    return heights;
}

void math::FastFourierTransform::generateInitialSpectrum()
{
    std::vector<float> kx(N), ky(N);
    for (int i = 0; i < N; ++i) {
        kx[i] = (i < N / 2 ? i : i - N) * (2 * M_PI / L_patch);
        ky[i] = kx[i];
    }

    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0f, 1.0f);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int idx = i * N + j;
            float kx_val = kx[j];
            float ky_val = ky[i];
            float k = std::sqrt(kx_val * kx_val + ky_val * ky_val);
            if (k < 1e-6f) k = 1e-6f;

            float L_w = wind_speed * wind_speed / g;
            float k_dot_w = (kx_val * wind_dir[0] + ky_val * wind_dir[1]) / k;
            float P = A * std::exp(-1.0f / (k * L_w) * (k * L_w)) / (k * k * k * k);
            P *= (k_dot_w * k_dot_w);
            P *= std::exp(-k * k * 0.001f * 0.001f);

            float real_part = distribution(generator) * std::sqrt(P / 2.0f);
            float imag_part = distribution(generator) * std::sqrt(P / 2.0f);
            h0[idx] = { real_part, imag_part };

            omega[idx] = std::sqrt(g * k);
        }
    }
}

    
    