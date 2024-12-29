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

        CubemapEctTexture(const std::string filename);

        ~CubemapEctTexture();

        virtual void Load();
        

        virtual void Bind(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, int layoutIndex);
        void LoadCubemapData(const TextureInputChunk& info);
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
        std::vector<Bitmap> cubemap;
        
        void populate();
        void make_view(int numberOfArray);
        void make_sampler();
        void make_descriptor_set();
       

    };
}
