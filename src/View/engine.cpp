#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "View/egnine.h"
#include "View/vkInit/instance.h"
#include <View/vkInit/logging.h>

#include <View/vkInit/device.h>
#include <View/vkInit/swapchain.h>


GraphicsEngine::GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode) {
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}


	make_instance();
	choice_device();
}

GraphicsEngine::~GraphicsEngine() {
	
	device.waitIdle();
	if (debugMode) {
		std::cout << "End!\n";
	}

	resourcesManager.clean(device);
	cleanup_swapchain();
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.destroy();

	}
	device.destroy();

	instance.destroySurfaceKHR(surface);

	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	instance.destroy();
	glfwTerminate();
}

void GraphicsEngine::make_instance() {
	this->instance = vkInit::make_instance(this->debugMode, this->appName);
	this->dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
	if (debugMode) {
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	}
	VkSurfaceKHR c_style_surface;
	if (glfwCreateWindowSurface(instance, mainWindow, nullptr, &c_style_surface) != VK_SUCCESS) {
		if (debugMode) {
			std::cout << "Failed to abstract glfw surface for Vulkan\n";
		}
	}
	else if (debugMode) {
		std::cout << "Successfully abstracted glfw surface for Vulkan\n";
	}
	surface = c_style_surface;
	
}

void GraphicsEngine::choice_device() {
	this->physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	this->device = vkInit::create_logical_device(physicalDevice, surface, debugMode);

	std::array<vk::Queue, 3> queues = vkInit::get_Queues(physicalDevice, device, surface, debugMode);

	this->graphicsQueue = queues[0];

	this->presentQueue = queues[1];

	this->computeQueue = queues[2];

	this->create_swapchain();

	frameNumber = 0;
}

void GraphicsEngine::create_swapchain()
{
	vkInit::SwapChainBundle bundle = vkInit::create_swapchain(physicalDevice, device, surface, screenSize, debugMode);
	this->swapchain = bundle.swapchain;
	this->swapchainFrames = bundle.frames;
	this->swapchainFormat = bundle.format;
	this->swapchainExtent = bundle.extent;

	vkInit::query_swapchain_support(physicalDevice, surface, debugMode);
	maxFramesInFlight = static_cast<int>(swapchainFrames.size());

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.logicalDevice = device;
		frame.physicalDevice = physicalDevice;
		frame.width = swapchainExtent.width;
		frame.height = swapchainExtent.height;
		
	}
}

void GraphicsEngine::recreate_swapchain(Scene* scene){
	this->screenSize.x = 0;
	this->screenSize.y = 0;
	while (this->screenSize.x == 0 || this->screenSize.y == 0) {
		glfwGetFramebufferSize(mainWindow, &this->screenSize.x, &this->screenSize.y);
		glfwWaitEvents();
	}

	device.waitIdle();

	cleanup_swapchain();
	create_swapchain();
	//create_framebuffers();
	//create_frame_resources(scene);
	//vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool,computeCommandPool ,swapchainFrames };
	//vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
}

void GraphicsEngine::cleanup_swapchain() {

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.destroy();
	}
	device.destroySwapchainKHR(swapchain);
}
