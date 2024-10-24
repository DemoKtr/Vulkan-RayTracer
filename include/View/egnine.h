#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "config.h"
#include <Scene/scene.h>
#include <Player/camera.h>
#include "View/vkUtil/frame.h"
#include "settings.h"
#include "View/Interface/interface.h"
#include "View/vkMesh/meshTypes.h"

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

	vk::DescriptorPool imguiDescriptorPool;
	vk::RenderPass imguiRenderPass;
	vk::CommandBuffer maincommandBuffer;
	vk::CommandPool imguiCommandPool;
	vk::CommandPool CommandPool;

	

	resourceManager resourcesManager{};

	editor* sceneEditor;

	modelNames  meshes;

	void make_instance(); //instance Setup
	void choice_device();
	void create_swapchain();
	void recreate_swapchain(Scene* scene);
	void cleanup_swapchain();
	void create_descriptor_set_layouts();
	void create_pipeline();
	void finalize_setup(Scene* scene);
	void make_assets(Scene* scene);
	void create_imgui_resources();
	void create_imgui_descriptor_pool();
	void create_frame_command_buffer();
	void create_frame_resources(Scene* scene);
	void create_framebuffers();

	
public:
	GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode);
	void render(Scene* scene, int& verticesCounter, float deltaTime, Camera::Camera camera, bool renderIMGUI);
	void InitImGui(GLFWwindow* window);
	~GraphicsEngine();
};