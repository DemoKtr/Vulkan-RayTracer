#pragma once
#include "config.h"
#include <GLFW/glfw3.h>
#include <Scene/scene.h>
namespace vkPrefab {
	struct PrefabRendererInput {

	};
	class PrefabRenderer {

		bool debugMode{ false };

		vk::Instance instance{ nullptr }; //instancja
		
		vk::PhysicalDevice physicalDevice{ nullptr };
		//logical device
		vk::Device device{ nullptr };
		//queues
		vk::Queue graphicsQueue{ nullptr };	
		//surface
		vk::SurfaceKHR surface;
		//swapchain
		vk::SwapchainKHR swapchain;
		//frames
		//std::vector<vkUtil::SwapChainFrame> swapchainFrames;
		//swapchainFormat
		vk::Format swapchainFormat;
		//swapchainExtent
		vk::Extent2D swapchainExtent;
		//DescriptorPool
		vk::DescriptorPool imguiDescriptorPool;



		void finalize_setup(Scene* scene);
		
	public:
		PrefabRenderer(PrefabRendererInput input);
		void render(Scene* scene, int& verticesCounter, float deltaTime, bool renderIMGUI);
		void InitImGui(GLFWwindow* window);
	};
}
