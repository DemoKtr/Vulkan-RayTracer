#include "View/Interface/PrefabWindow/PrefabRenderer.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <View/vkUtil/queues.h>
#include <View/vkInit/descrpitors.h>
#include <View/vkInit/swapchain.h>


void vkPrefab::PrefabRenderer::create_swapchain() {
	VkSurfaceKHR c_style_surface;
	if (glfwCreateWindowSurface(instance, window, nullptr, &c_style_surface) != VK_SUCCESS) {
		if (debugMode) {
			std::cout << "Failed to abstract glfw surface for Vulkan\n";
		}
	}
	else if (debugMode) {
		std::cout << "Successfully abstracted glfw surface for Vulkan\n";
	}
	this->surface = c_style_surface;
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
		frame.make_depth_resources();

	}
	frameNumber = 0;

}

void vkPrefab::PrefabRenderer::create_frame_resources() {

}

void vkPrefab::PrefabRenderer::create_descriptors_set_layouts() {

}

void vkPrefab::PrefabRenderer::create_pipeline() {

}

void vkPrefab::PrefabRenderer::make_assets(Scene* scene) {

	projection = vkRenderStructs::getProjectionMatrix(swapchainExtent);
	meshesManager = new vkMesh::MeshesManager(scene->root, scene->ecs);
	
}

void vkPrefab::PrefabRenderer::finalize_setup(Scene* scene) {

}

void vkPrefab::PrefabRenderer::recreate_swapchain(Scene* scene) {
	this->screenSize.x = 0;
	this->screenSize.y = 0;
	while (this->screenSize.x == 0 || this->screenSize.y == 0) {
		glfwGetFramebufferSize(window, &this->screenSize.x, &this->screenSize.y);
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

void vkPrefab::PrefabRenderer::cleanup_swapchain() {
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.destroy();
	}
	device.destroySwapchainKHR(swapchain);
}

vkPrefab::PrefabRenderer::PrefabRenderer(PrefabInput input,Scene* scene ,GLFWwindow* Window) {
	/*
	struct PrefabInput {
		glm::ivec2 screenSize;
		
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
	*/

	this->screenSize = input.screenSize;
	this->window = Window;
	this->instance = input.instance;
	this->physicalDevice = input.physicalDevice;
	this->device = input.device;
	this->graphicsQueue = input.graphicsQueue;
	this->meshes = input.meshes;
	this->atlasTextures = input.atlasTextures;
	create_swapchain();
	create_descriptors_set_layouts();
	create_pipeline();
	finalize_setup(scene);
	make_assets(scene);
}

void vkPrefab::PrefabRenderer::render(Scene* scene, Camera::Camera camera, float deltaTime, bool renderIMGUI)
{
}

void vkPrefab::PrefabRenderer::InitImGui(GLFWwindow* window) {
	imguiDescriptorPool = vkInit::make_imgui_descriptor_pool(device);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	io.DisplaySize = ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight));

	// Ustaw styl
	ImGui::StyleColorsDark();

	// 2. Inicjalizacja ImGui dla GLFW
	ImGui_ImplGlfw_InitForVulkan(window, true);
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = physicalDevice;
	init_info.Device = device;
	init_info.QueueFamily = indices.graphicsFamily.value();
	init_info.Queue = graphicsQueue;
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = imguiDescriptorPool; // twoja pula deskryptorów ;
	init_info.MinImageCount = 2; // minimalna liczba obrazów ;
	//init_info.ImageCount = swapchainFrames.size(); // liczba obrazów w swap chain ;
	init_info.CheckVkResultFn = nullptr; // Mo¿esz przypisaæ swoj¹ funkcjê do obs³ugi b³êdów
	//init_info.RenderPass = imguiRenderPass;
	ImGui_ImplVulkan_Init(&init_info);

	// Przyk³ad tworzenia deskryptora dla ImGui
	//ImGui_ImplVulkan_CreateFontsTexture(vkCommandBuffer);
}
