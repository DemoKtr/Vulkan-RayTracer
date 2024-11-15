#pragma once
#include "config.h"
struct resourceManager {
    std::vector<vk::CommandPool> commandPools = {};
    std::vector<vk::Pipeline> pipelines = {};
    std::vector<vk::PipelineLayout> pipelineLayouts = {};
    std::vector<vk::RenderPass> renderpasses = {};
    std::vector<vk::DescriptorSet> descriptorSets = {};
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = {};
    std::vector<vk::DescriptorPool> descriptorPools = {};
    std::vector<vk::ImageView> imageViews = {};
    std::vector<vk::Image> images = {};
    std::vector<vk::Framebuffer> framebuffers = {};
    std::vector<vk::Semaphore> semaphores = {};
    std::vector<vk::Fence> fences = {};
    std::vector<vk::Buffer> buffers = {};
    std::vector<vk::DeviceMemory> bufferMemories = {};
    std::vector<vk::Sampler> samplers = {};

    void add(vk::CommandPool commandPool) {
        commandPools.push_back(commandPool);
    }

    void add(vk::Pipeline pipeline) {
        pipelines.push_back(pipeline);
    }

    void add(vk::PipelineLayout pipelineLayout) {
        pipelineLayouts.push_back(pipelineLayout);
    }

    void add(vk::RenderPass renderPass) {
        renderpasses.push_back(renderPass);
    }

    void add(vk::DescriptorSet descriptorSet) {
        descriptorSets.push_back(descriptorSet);
    }

    void add(vk::DescriptorSetLayout descriptorSetLayout) {
        descriptorSetLayouts.push_back(descriptorSetLayout);
    }

    void add(vk::DescriptorPool descriptorPool) {
        descriptorPools.push_back(descriptorPool);
    }

    void add(vk::ImageView imageView) {
        imageViews.push_back(imageView);
        std::cout << imageViews.size() << std::endl;
    }

    void add(vk::Image image) {
        images.push_back(image);
        std::cout << images.size() << std::endl;
    }

    void add(vk::Framebuffer framebuffer) {
        framebuffers.push_back(framebuffer);
    }

    void add(vk::Semaphore semaphore) {
        semaphores.push_back(semaphore);
    }

    void add(vk::Fence fence) {
        fences.push_back(fence);
    }

    void add(vk::Buffer buffer) {
        buffers.push_back(buffer);
    }

    void add(vk::DeviceMemory bufferMemory) {
        bufferMemories.push_back(bufferMemory);
    }

    void add(vk::Sampler sampler) {
        samplers.push_back(sampler);
    }


    void clean(vk::Device device) {

        

        for (vk::Sampler sampler : samplers) {
            device.destroySampler(sampler);
        }
        for (vk::DeviceMemory bufferMemory : bufferMemories) {
            device.unmapMemory(bufferMemory);
            device.freeMemory(bufferMemory);
        }
        for (vk::ImageView imageView : imageViews) {
            device.destroyImageView(imageView);
        }
        for (vk::Image image : images) {
            device.destroyImage(image);
        }
        for (vk::Framebuffer framebuffer : framebuffers) {
            device.destroyFramebuffer(framebuffer);
        }
        for (vk::Semaphore semaphore : semaphores) {
            device.destroySemaphore(semaphore);
        }
        for (vk::Fence fence : fences) {
            device.destroyFence(fence);
        }
        for (vk::Buffer buffer : buffers) {
            device.destroyBuffer(buffer);
        }
        for (vk::DescriptorPool descriptorPool : descriptorPools) {
            device.destroyDescriptorPool(descriptorPool);
        }
        for (vk::CommandPool commandPool : commandPools) {
            device.destroyCommandPool(commandPool);
        }
        for (vk::Pipeline pipeline : pipelines) {
            device.destroyPipeline(pipeline);
        }
        for (vk::PipelineLayout pipelineLayout : pipelineLayouts) {
            device.destroyPipelineLayout(pipelineLayout);
        }
        for (vk::RenderPass renderPass : renderpasses) {
            device.destroyRenderPass(renderPass);
        }
        for (vk::DescriptorSetLayout descriptorSetLayout : descriptorSetLayouts) {
            device.destroyDescriptorSetLayout(descriptorSetLayout);
        }



    }
};
namespace vkSettings {
	static float zNear = 0.1f;
	static float zFar = 64.0f;
	static float lightFOV = 100.0f;
	static bool firstMouse = true;
	static float lastX = { 0.0f };
	static float lastY = { 0.0f };
    static int MIN_TRIANGLES = 2; // Minimalna liczba trójk¹tów w wêŸle BVH
    static int MAX_DEPTH = 32;     // Maksymalna g³êbokoœæ drzewa BVH
    static int thumbWidth = 512;
    static int thumbHeigh = 512;
    


}