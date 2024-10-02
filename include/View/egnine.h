#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "config.h"
#include <Scene/scene.h>
#include <Player/camera.h>
#include "View/vkUtil/frame.h"

class GraphicsEngine {
	glm::ivec2 screenSize;
	const char* appName{ "RayTracer" };
	//synchronizers 
	int maxFramesInFlight, frameNumber;
	GLFWwindow* mainWindow;
	bool debugMode{true};
	vk::Instance instance{ nullptr }; //instancja
	//debug callback
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	//dynamic instance dispatcher
	vk::DispatchLoaderDynamic dldi;
	//surface
	vk::SurfaceKHR surface;
	//physical device
	vk::PhysicalDevice physicalDevice{ nullptr };
	//logical device
	vk::Device device{ nullptr };
	//queues
	vk::Queue graphicsQueue{ nullptr };
	vk::Queue presentQueue{ nullptr };
	vk::Queue computeQueue{ nullptr };
	

	//swapchain
	vk::SwapchainKHR swapchain;
	//frames
	std::vector<vkUtil::SwapChainFrame> swapchainFrames;
	//swapchainFormat
	vk::Format swapchainFormat;
	//swapchainExtent
	vk::Extent2D swapchainExtent;

	void make_instance(); //instance Setup
	void choice_device();
	void create_swapchain();
	void recreate_swapchain(Scene* scene);
	void cleanup_swapchain();

public:
	GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode);
	void render(Scene* scene, int& verticesCounter, float deltaTime, Camera::Camera camera);
	~GraphicsEngine();
};