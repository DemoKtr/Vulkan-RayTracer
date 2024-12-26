#pragma once
#include "config.h"
#include <View/vkImage/image.h>
#include <View/vkImage/bitmap.h>

namespace vkImage{
    class BaseCubmapTexture {
    public:

        virtual void Load() = 0;

        virtual void Bind(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, int layoutIndex) = 0;

    };


    // Ect - Equirectangular
    class CubemapEctTexture : public BaseCubmapTexture
    {
    public:

        CubemapEctTexture(const TextureInputChunk& info);

        ~CubemapEctTexture() {};

        virtual void Load();
        

        virtual void Bind(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, int layoutIndex);

    private:
        int width, height, channels;

        vk::Device device;
        vk::PhysicalDevice physicalDevice;
        std::string filename;
        vk::CommandBuffer commandBuffer;
        vk::Queue queue;

        vk::DescriptorSet descriptorSet;
        vk::DescriptorSetLayout descriptorLayout;
        vk::DescriptorPool descriptorPool;

        vk::Image image;
        vk::DeviceMemory imageMemory;
        vk::ImageView imageView;
        vk::Sampler sampler;

        void LoadCubemapData(std::vector<Bitmap>& Cubemap);
        void populate(std::vector<Bitmap>& Cubemap);
        void make_view(int numberOfArray);
        void make_sampler();
        void make_descriptor_set();
       

    };
}
