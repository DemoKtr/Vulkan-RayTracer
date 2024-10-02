#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "View/egnine.h"
#include "View/vkInit/instance.h"
#include <View/vkInit/logging.h>
#include "settings.h"
#include <View/vkInit/device.h>


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

	vkSettings::resourcesManager.clean(device);
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
}
