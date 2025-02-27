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
        // Konstruktor przyjmuj¹cy rozmiary objêtoœci mg³y
        
        Fog(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::CommandBuffer commandBuffer, vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool, int width, int height, int depth)
            : width(width), height(height), depth(depth)
        {
            generateData(physicalDevice,device,queue,commandBuffer,layout,descriptorPool);
        }

        // Zwraca referencjê do wygenerowanych danych mg³y
      

        // Gettery do rozmiarów objêtoœci
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getDepth() const { return depth; }

        void render(vk::CommandBuffer commandBuffer, vk::Extent2D swapchainExtent, vk::ImageView& imageView, vk::DescriptorSet& dset);

    private:
        int width, height, depth;
      

        // Funkcja smoothstep – p³ynne przejœcie miêdzy wartoœciami
        float smoothstep(float edge0, float edge1, float x) const {
            float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            return t * t * (3.0f - 2.0f * t);
        }

        // Funkcja generuj¹ca dane mg³y w postaci std::vector<float>
        // Zak³adamy, ¿e mg³a mieœci siê w jednostkowym szeœcianie [0,1]^3.
        void generateData(vk::PhysicalDevice physicalDevice,
            vk::Device device, vk::Queue queue, 
            vk::CommandBuffer commandBuffer, 
            vk::DescriptorSetLayout layout, 
            vk::DescriptorPool descriptorPool);

    };
}
