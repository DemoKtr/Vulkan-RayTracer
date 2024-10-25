#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "View/egnine.h"
#include "View/vkInit/instance.h"
#include <View/vkInit/logging.h>

#include <View/vkInit/device.h>
#include <View/vkInit/swapchain.h>
#include <View/vkInit/descrpitors.h>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <View/vkUtil/queues.h>
#include <View/vkUtil/renderpass.h>
#include <View/vkInit/commands.h>
#include <View/vkInit/synchronizer.h>
#include <View/vkInit/framebuffer.h>

#include "View/vkMesh/meshLoader.h"

GraphicsEngine::GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode) {
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}


	make_instance();
	choice_device();
	//create descriptor set layout
	//create pipeline
	finalize_setup(scene);
	make_assets(scene);

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
	delete sceneEditor;
}

void GraphicsEngine::create_frame_resources(Scene* scene) {
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{

		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
		frame.computeFinished = vkInit::make_semaphore(device, debugMode);
		frame.inFlight = vkInit::make_fence(device, debugMode);
	}
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
	frameNumber = 0;
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


void GraphicsEngine::create_imgui_resources(){
	create_imgui_descriptor_pool();

}

void GraphicsEngine::create_imgui_descriptor_pool() {
	
	imguiDescriptorPool = vkInit::make_imgui_descriptor_pool(device);
}

void GraphicsEngine::create_frame_command_buffer() {
	
	CommandPool = vkInit::make_command_pool(physicalDevice, device, surface, debugMode);
	imguiCommandPool = vkInit::make_command_pool(physicalDevice, device, surface, debugMode);
	vkInit::commandBufferInputChunk commandBufferInput = { device,CommandPool ,imguiCommandPool, swapchainFrames };
	vkInit::commandBufferOutput output = vkInit::make_command_buffer(commandBufferInput, debugMode);
	maincommandBuffer = output.graphicCommandBuffer;
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
}

void GraphicsEngine::InitImGui(GLFWwindow* window){
	
	create_imgui_resources();
	
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
	init_info.ImageCount = swapchainFrames.size(); // liczba obrazów w swap chain ;
	init_info.CheckVkResultFn = nullptr; // Mo¿esz przypisaæ swoj¹ funkcjê do obs³ugi b³êdów
	init_info.RenderPass = imguiRenderPass;
	ImGui_ImplVulkan_Init(&init_info);

	// Przyk³ad tworzenia deskryptora dla ImGui
	//ImGui_ImplVulkan_CreateFontsTexture(vkCommandBuffer);
	
}

void GraphicsEngine::finalize_setup(Scene* scene){
	imguiRenderPass = vkUtil::create_imgui_renderpass(device, swapchainFormat);
	create_framebuffers();
	create_frame_resources(scene);
	create_frame_command_buffer();
}

void GraphicsEngine::create_framebuffers(){
	vkInit::framebufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = imguiRenderPass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(frameBufferInput, swapchainFrames, debugMode);
}

void GraphicsEngine::render(Scene* scene, int& verticesCounter, float deltaTime, Camera::Camera camera,bool renderIMGUI) {
	
	if (renderIMGUI) {
		
	device.waitForFences(1, &swapchainFrames[frameNumber].inFlight, VK_TRUE, UINT64_MAX);
	device.resetFences(1, &swapchainFrames[frameNumber].inFlight);


	uint32_t imageIndex;
	try {
		vk::ResultValue acquire = device.acquireNextImageKHR(
			swapchain, UINT64_MAX,
			swapchainFrames[frameNumber].imageAvailable, nullptr
		);
		imageIndex = acquire.value;
	}
	catch (vk::OutOfDateKHRError error) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain(scene);
		return;
	}
	catch (vk::IncompatibleDisplayKHRError error) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain(scene);
		return;
	}
	catch (vk::SystemError error) {
		std::cout << "Failed to acquire swapchain image!" << std::endl;
	}


	
	vk::CommandBuffer imgcommandBuffer = swapchainFrames[frameNumber].imguiCommandBuffer;

	imgcommandBuffer.reset();

	
	//render_imgui(imgcommandBuffer,frameNumber,debugMode);
	sceneEditor->render_editor(imgcommandBuffer,imguiRenderPass,swapchainFrames,meshesNames,swapchainExtent,frameNumber,debugMode);

	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNumber].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &imgcommandBuffer;

	vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNumber].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try {
		graphicsQueue.submit(submitInfo, swapchainFrames[frameNumber].inFlight);
	}
	catch (vk::SystemError err) {
		std::cout << "Synchronization failed" << std::endl;
	}

	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	vk::SwapchainKHR swapChains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vk::Result present;

	try {
		present = presentQueue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError error) {
		present = vk::Result::eErrorOutOfDateKHR;
	}

	if (present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain(scene);
		return;
	}

	frameNumber = (frameNumber + 1) % maxFramesInFlight;
	}
}

void GraphicsEngine::make_assets(Scene* scene) {
	meshes = new vkMesh::VertexMenagerie();
	sceneEditor = new editor(scene);
	listMeshesFilesInDirectory("\\core", meshesNames);
	std::vector<vkMesh::MeshLoader> test;
	for (std::string path : meshesNames.fullPaths) {
		vkMesh::MeshLoader m(path.c_str());
		test.push_back(m);
	}
	size_t index = 0;
	for (vkMesh::MeshLoader m : test) {
		vkMesh::VertexBuffers buffer = m.getData();
		meshes->consume(index++,buffer.vertices,buffer.indicies);
	}
	
}

