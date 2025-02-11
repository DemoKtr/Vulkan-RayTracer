#pragma once
#include <vector>
#include <complex>
#include "config.h"
#include "View/vkImage/texture.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>
#include <random>
#include <View/vkUtil/frame.h>
#include "View/RenderStructs/projection.h"
namespace math {
	

	class FastFourierTransform {
	public:
		vk::PhysicalDevice physicalDevice;
		vk::Device device;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;

		vkRenderStructs::ViewProjectionData vp;
		vkImage::Texture* heighMapTexture;
		
		std::vector<std::complex<float>> currentFFTData;  // 1D array zamiast 2D
        FastFourierTransform(int N, float L_patch, float wind_speed, glm::vec2 wind_dir, float A, float g);
		~FastFourierTransform();
      
		void createTexture(vk::PhysicalDevice physicalDevice,vk::Device device, vk::CommandBuffer commandBuffer, vk::Queue queue,vk::DescriptorSetLayout layout, vk::DescriptorPool descriptorPool);
        void update(float t);
		void Render(vkUtil::SwapChainFrame& frame, vk::CommandBuffer& commandBuffer, vk::Extent2D& swapchainExtent, vk::DispatchLoaderDynamic& dldi);
		const std::vector<float>& getHeights() const;

    private:
        int N;
        float L_patch;
        float wind_speed;
        float A;
        float g;
        glm::vec2 wind_dir;
        std::vector<std::complex<float>> h0;
        std::vector<float> omega;
        std::vector<float> heights;

        void generateInitialSpectrum();
	
	};

}