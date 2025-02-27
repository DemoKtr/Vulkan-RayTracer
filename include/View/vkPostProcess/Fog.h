#pragma once
#include "config.h"
#include "View/vkImage/texture3D.h"


#define STB_PERLIN_IMPLEMENTATION
#include "MathFunctions/stb_perlin.h"



namespace postprocess {
    
    struct FogUBO {
        glm::mat4 uInvViewProjection;
        glm::vec4 uCameraPos;
        glm::vec4 uBoxMin;
        glm::vec4 uBoxMax;
        float uTime;
        float padding[3];
    };
    class Fog {
    public:
        
        vkImage::Texture3D* texture;
        // Konstruktor przyjmuj�cy rozmiary obj�to�ci mg�y
        
        Fog(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::CommandBuffer commandBuffer, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool, int width, int height, int depth)
            : width(width), height(height), depth(depth)
        {
            generateData(physicalDevice,device,queue,commandBuffer,layout,descriptorPool);
        }

        // Zwraca referencj� do wygenerowanych danych mg�y
      

        // Gettery do rozmiar�w obj�to�ci
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getDepth() const { return depth; }

        void render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet& dset);

    private:
        int width, height, depth;
      

        // Funkcja smoothstep � p�ynne przej�cie mi�dzy warto�ciami
        float smoothstep(float edge0, float edge1, float x) const {
            float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            return t * t * (3.0f - 2.0f * t);
        }

        // Funkcja generuj�ca dane mg�y w postaci std::vector<float>
        // Zak�adamy, �e mg�a mie�ci si� w jednostkowym sze�cianie [0,1]^3.
        void generateData(vk::PhysicalDevice physicalDevice,
            vk::Device device, vk::Queue queue, 
            vk::CommandBuffer commandBuffer, 
            vk::DescriptorSetLayout layout, 
            vk::DescriptorPool descriptorPool);

    };
}
