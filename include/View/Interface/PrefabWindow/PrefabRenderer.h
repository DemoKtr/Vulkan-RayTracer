#pragma once
#include "config.h"
#include <GLFW/glfw3.h>
#include <Scene/scene.h>
#include "Player/camera.h"
namespace vkPrefab {
	struct PrefabInput {
		GLFWwindow* mainWindow;
		bool debugMode;
		vk::Instance instance;
		//physical device
		vk::PhysicalDevice physicalDevice;
		//logical device
		vk::Device device;
		//queues
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::Queue computeQueue;
		vk::Queue transferQueue;
	};
	class PrefabRenderer {

		bool debugMode{ false };
		GLFWwindow* window;
		vk::Instance instance{ nullptr }; //instancja
		
		vk::PhysicalDevice physicalDevice{ nullptr };
		//logical device
		vk::Device device{ nullptr };
		//queues
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::Queue computeQueue;
		vk::Queue transferQueue;
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
		PrefabRenderer(PrefabInput input);
		void render(Scene* scene,Camera::Camera camera,float deltaTime, bool renderIMGUI);
		void InitImGui(GLFWwindow* window);
	};
}
