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
#include <View/vkInit/vkPipeline/pipelineBuilder.h>
#include <View/vkMesh/vertexFormat.h>
#include <Scene/ECS/components/components.h>
#include "View/vkInit/vkAccelerationStructures/AccelerationStructure.h"
#include "View/vkInit/vkExtensionsFunctions/BufferAdress.h"
#include "fileOperations/resources.h"
#include "View/vkResources/resources.h"
#include "Scene/Objects/PrefabResources.h"
#include "MultithreatedSystems/TaskManager.h"
#include "fileOperations/filesTypes.h"
#include <MultithreatedSystems/mutexManager.h>


GraphicsEngine::GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode) {
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}
	

	particleManager = new vkParticle::ParticleManager();
	auto& taskmanager = TaskManager::getInstance();
	taskmanager.initialize();

	taskmanager.submitTask(
		TaskPriority::HIGH,
		[this](auto&&... args) { initial_cubemap(std::forward<decltype(args)>(args)...); }
	);

	auto& manager = MutexManager::getInstance();

	manager.addMutex("Descriptors");

	make_instance();
	choice_device();

	vkResources::scenePipelines = new vkUtil::PipelineCache(device);
	vkResources::editorPipelines = new vkUtil::PipelineCache(device);

	test = UImanager.create_button(glm::vec2(0,0), glm::vec2(300, 200));

	create_descriptor_set_layouts();

	create_pipeline();

	finalize_setup(scene);
	
	
	make_assets(scene);

	vkPrefab::data.physicalDevice = physicalDevice;
	vkPrefab::data.device = this->device;
	vkPrefab::data.instance = this->instance;
	vkPrefab::data.screenSize = this->screenSize;
	vkPrefab::data.graphicsQueue = this->graphicsQueue;
	vkPrefab::data.presentQueue = this->presentQueue;
	vkPrefab::data.computeQueue = this->computeQueue;
	
	
}

GraphicsEngine::~GraphicsEngine() {
	UImanager.remove_button(test);
	UImanager.remove_text(testText);
	device.waitIdle();
	if (debugMode) {
		std::cout << "End!\n";
	}
	delete vkResources::scenePipelines;
	delete vkResources::editorPipelines;

	delete sceneEditor;
	delete vkResources::meshes;
	delete meshesManager;
	delete vkResources::atlasTextures;
	delete particleManager;

	//delete cubemap;
	device.destroyCommandPool(CommandPool);
	device.destroyCommandPool(computeCommandPool);
	device.destroyCommandPool(transferCommandPool);


	device.destroyDescriptorSetLayout(iconDescriptorSetLayout);
	device.destroyDescriptorSetLayout(postprocessDescriptorSetLayout);
	device.destroyDescriptorSetLayout(textureDescriptorSetLayout);
	device.destroyDescriptorSetLayout(cubemapDescriptorSetLayout);
	device.destroyDescriptorSetLayout(UIDescriptorSetLayout);
	device.destroyDescriptorSetLayout(UIFontSBODescriptorSetLayout);
	device.destroyDescriptorSetLayout(UIFontTextureDescriptorSetLayout);

	

	device.destroyDescriptorPool(postprocessDescriptorPool);
	device.destroyDescriptorPool(iconDescriptorPool);
	device.destroyDescriptorPool(imguiDescriptorPool);
	device.destroyDescriptorPool(textureDescriptorPool);
	device.destroyDescriptorPool(cubemapDescriptorPool);
	device.destroyDescriptorPool(UIDescriptorPool);
	device.destroyDescriptorPool(UIFontSBODescriptorPool);
	device.destroyDescriptorPool(UIFontTextureDescriptorPool);
	
	
	cleanup_swapchain();
	
	device.destroy();

	instance.destroySurfaceKHR(surface);

	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	instance.destroy();
	glfwTerminate();
	
}

void GraphicsEngine::create_frame_resources(int number_of_models) {

	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	postprocessDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);
	bindings.count = 1;
	bindings.types[0] = vk::DescriptorType::eStorageBuffer;
	UIDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);
	UIFontSBODescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
		frame.computeFinished = vkInit::make_semaphore(device, debugMode);
		frame.inFlight = vkInit::make_fence(device, debugMode); 
		frame.make_descriptors_resources(number_of_models);
		frame.postprocessDescriptorSet = vkInit::allocate_descriptor_set(device, postprocessDescriptorPool, postprocessDescriptorSetLayout);
		frame.UIDescriptorSet = vkInit::allocate_descriptor_set(device, UIDescriptorPool, UIDescriptorSetLayout);
		frame.UIFontDescriptorSet = vkInit::allocate_descriptor_set(device, UIFontSBODescriptorPool, UIFontSBODescriptorSetLayout);
	}
}

void GraphicsEngine::create_pipeline() {

	vkUtil::PipelineCacheChunk pipeline;
	pipeline.type = 0;

	vkInit::PipelineBuilder pipelineBuilder(device);


	pipelineBuilder.set_overwrite_mode(true);
	pipelineBuilder.specify_vertex_format(
		vkMesh::getVertexInputBindingDescription(),
		vkMesh::getVertexInputAttributeDescription());
	pipelineBuilder.specify_vertex_shader("resources/shaders/vert.spv");
	pipelineBuilder.specify_fragment_shader("resources/shaders/frag.spv");
	pipelineBuilder.specify_swapchain_extent(swapchainExtent);
	pipelineBuilder.clear_depth_attachment();
	pipelineBuilder.set_color_blending(false);
	pipelineBuilder.add_descriptor_set_layout(postprocessDescriptorSetLayout);
	pipelineBuilder.add_descriptor_set_layout(textureDescriptorSetLayout);
	pipelineBuilder.use_depth_test(true);
	pipelineBuilder.setPushConstants(sizeof(vkRenderStructs::ProjectionData),1);
	pipelineBuilder.dynamicRendering = true;
	vkInit::GraphicsPipelineOutBundle output = pipelineBuilder.build(swapchainFormat,swapchainFrames[0].depthFormat);

	pipeline.pipelineLayout = output.layout;
	pipeline.pipeline = output.pipeline;

	vkResources::scenePipelines->addPipeline("Unlit Pipeline", pipeline);
	pipelineBuilder.reset();



	pipelineBuilder.set_overwrite_mode(true);
	pipelineBuilder.specify_vertex_shader("resources/shaders/UIvert.spv");
	pipelineBuilder.specify_fragment_shader("resources/shaders/UIfrag.spv");
	pipelineBuilder.specify_swapchain_extent(swapchainExtent);
	pipelineBuilder.clear_depth_attachment();
	pipelineBuilder.add_descriptor_set_layout(UIDescriptorSetLayout);
	pipelineBuilder.use_depth_test(false);
	pipelineBuilder.set_color_blending(false);
	pipelineBuilder.dynamicRendering = true;
	pipelineBuilder.setPushConstants(sizeof(glm::vec2), 1);
	output = pipelineBuilder.build(swapchainFormat, swapchainFrames[0].depthFormat);
	pipeline.pipelineLayout = output.layout;
	pipeline.pipeline = output.pipeline;
	vkResources::scenePipelines->addPipeline("UI Pipeline", pipeline);
	pipelineBuilder.reset();

	pipelineBuilder.set_overwrite_mode(true);
	pipelineBuilder.specify_vertex_shader("resources/shaders/UIFontvert.spv");
	pipelineBuilder.specify_fragment_shader("resources/shaders/UIFontfrag.spv");
	pipelineBuilder.specify_swapchain_extent(swapchainExtent);
	pipelineBuilder.clear_depth_attachment();
	pipelineBuilder.add_descriptor_set_layout(UIFontSBODescriptorSetLayout);
	pipelineBuilder.add_descriptor_set_layout(UIFontTextureDescriptorSetLayout);
	pipelineBuilder.use_depth_test(false);
	pipelineBuilder.dynamicRendering = true;
	pipelineBuilder.setPushConstants(sizeof(glm::vec2), 1);
	pipelineBuilder.set_color_blending(true);
	output = pipelineBuilder.build(swapchainFormat, swapchainFrames[0].depthFormat);
	pipeline.pipelineLayout = output.layout;
	pipeline.pipeline = output.pipeline;
	vkResources::scenePipelines->addPipeline("UIFont Pipeline", pipeline);
	pipelineBuilder.reset();
	//pipelineInfo = vkResources::scenePipelines->getPipeline("UIFont Pipeline");
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
	vkExtensions::Init(device);
	std::array<vk::Queue, 4> queues = vkInit::get_Queues(physicalDevice, device, surface, debugMode);

	this->graphicsQueue = queues[0];

	this->presentQueue = queues[1];

	this->computeQueue = queues[2];
	this->transferQueue = queues[3];


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
		frame.make_depth_resources();
		
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

void GraphicsEngine::create_descriptor_set_layouts() {
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;
	bindings.indices.push_back(0);
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	bindings.indices.push_back(1);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	postprocessDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);

	bindings.count = 1;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	textureDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);
	iconDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);
	cubemapDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);;
	UIFontTextureDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);;

	bindings.count = 1;
	bindings.types[0] = vk::DescriptorType::eStorageBuffer;
	bindings.stages[0] = vk::ShaderStageFlagBits::eVertex;
	UIDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);
	UIFontSBODescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


}

void GraphicsEngine::create_frame_command_buffer() {
	
	CommandPool = vkInit::make_command_pool(physicalDevice, device, surface, debugMode);
	computeCommandPool = vkInit::make_compute_command_pool(physicalDevice, device, surface, debugMode);
	transferCommandPool = vkInit::make_transfer_command_pool(physicalDevice, device, surface, debugMode);
	vkInit::commandBufferInputChunk commandBufferInput = { device,CommandPool, swapchainFrames };
	maincommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);
	vkInit::make_imgui_frame_command_buffers(commandBufferInput, debugMode);

	commandBufferInput.commandPool = computeCommandPool;
	computeCommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);
	commandBufferInput.commandPool = transferCommandPool;
	transferCommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);


	//vkInit::make_command_buffer(commandBufferInput, debugMode);
}

void GraphicsEngine::InitImGui(GLFWwindow* window){
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
	init_info.ImageCount = swapchainFrames.size(); // liczba obrazów w swap chain ;
	init_info.CheckVkResultFn = nullptr; // Mo¿esz przypisaæ swoj¹ funkcjê do obs³ugi b³êdów
	init_info.RenderPass = nullptr;
	init_info.UseDynamicRendering = VK_TRUE;
	init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	VkFormat format = VkFormat(swapchainFormat);
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
	init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;

	ImGui_ImplVulkan_Init(&init_info);

	
}

void GraphicsEngine::finalize_setup(Scene* scene){

	auto& taskmanager = TaskManager::getInstance();
	ThreadSafeResource<std::vector<vkUtil::SwapChainFrame>> sharedFrames;
	sharedFrames.set(swapchainFrames);
	/*
	taskmanager.submitTask(
		TaskPriority::HIGH,
		[this](auto&&... args) { create_frame_command_buffer(std::forward<decltype(args)>(args)...); }

	);
	*/
	
	taskmanager.submitTask(
		TaskPriority::LOW,
		[this](auto&&... args) { create_frame_resources(std::forward<decltype(args)>(args)...); },
		scene->getSceneObjectNumber(scene->root)
	);
	create_frame_command_buffer();
	//taskmanager.submitTask(TaskPriority::HIGH, create_frame_resources, std::ref(sharedFrames), scene->getSceneObjectNumber(scene->root));

}



void GraphicsEngine::load_meshes_files() {
	std::vector<std::string> ext = fileOperations::FileType::getExtensions(fileOperations::FileType::Model);
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	list_files_in_directory("\\core", filesManager.getMeshesNames(), ext);
}

void GraphicsEngine::load_textures_files() {
	
	std::vector<std::string> ext = fileOperations::FileType::getExtensions(fileOperations::FileType::Image);
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	fileOperations::list_files_in_directory("\\core", filesManager.getTexturesNames(), ext);
	this->load_fonts();
}

void GraphicsEngine::load_scripts() {
	std::vector<std::string> ext = { ".cpp", };
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	fileOperations::list_files_in_directory("\\core",filesManager.getCppNames(), ext);
	scripts::compileAllScripts(filesManager.getCppNames(), filesManager.getDllNames());
}

void GraphicsEngine::load_fonts() {
	std::vector<std::string> ext = fileOperations::FileType::getExtensions(fileOperations::FileType::Font);
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	fileOperations::list_files_in_directory("\\core", filesManager.getFontNames(), ext);
}

void GraphicsEngine::record_draw_command(vk::CommandBuffer commandBuffer, vk::CommandBuffer unlitCommandBuffer,Scene* scene ,uint32_t imageIndex) {
	

	vk::CommandBufferInheritanceInfo info;
	info.sType = vk::StructureType::eCommandBufferInheritanceInfo;
	info.renderPass = nullptr;
	info.framebuffer = nullptr;
	info.subpass = 0;
	info.pNext = nullptr;

	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	beginInfo.pInheritanceInfo = &info;

	// Sprawdzenie rozpoczêcia nagrywania command buffer
	try {
		commandBuffer.begin(beginInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin recording command buffer: " << err.what() << std::endl;
		return;
	}
	

	commandBuffer.executeCommands(unlitCommandBuffer);
	// Przygotowanie ImageMemoryBarrier do zmiany layoutu na VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	vk::ImageMemoryBarrier barrier = {};
	barrier.oldLayout = vk::ImageLayout::eUndefined; // obecny layout obrazu, np. undefined po stworzeniu
	barrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal; // nowy layout
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapchainFrames[imageIndex].mainimage;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor; // zakres aspektu obrazu (kolor)
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	// Okreœl dostêp pamiêci, aby przejœæ z poprzedniego do nowego layoutu
	barrier.srcAccessMask = {}; // Brak poprzednich operacji do synchronizacji
	barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite; // Docelowy dostêp, np. pisanie do attachmentu koloru

	// U¿yj vkCmdPipelineBarrier, aby zrealizowaæ barierê w command buffer
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTopOfPipe, // srcStageMask: najwczeœniejszy etap, brak poprzedniego u¿ycia
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // dstStageMask: docelowy etap, w którym obraz bêdzie u¿ywany
		{}, // flagi bariery
		nullptr, nullptr, // brak barier pamiêciowych ani buforowych
		barrier // wskaŸnik do bariery obrazu
	);

	UImanager.render_ui(commandBuffer,swapchainExtent, swapchainFrames[imageIndex].mainimageView, swapchainFrames[imageIndex].UIDescriptorSet, swapchainFrames[imageIndex].UIFontDescriptorSet, uiRenderingDrawData, fontManager);


	sceneEditor->render_editor(commandBuffer, swapchainFrames, &objects_to_rendering,swapchainExtent, imageIndex, debugMode);

	
	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
	
}

void GraphicsEngine::record_unlit_draw_command(vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
	
	vk::CommandBufferInheritanceInfo info;
	info.sType = vk::StructureType::eCommandBufferInheritanceInfo;
	info.renderPass = nullptr;
	info.framebuffer = nullptr;
	info.subpass = 0;
	info.pNext = nullptr;

	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
	beginInfo.pInheritanceInfo = &info;

	try {
		commandBuffer.begin(beginInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin recording command buffer: " << err.what() << std::endl;
		return;
	}


	// Debugging dla kolorów i za³¹czników
	std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vk::RenderingAttachmentInfoKHR colorAttachment = {};
	colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
	colorAttachment.imageView = swapchainFrames[imageIndex].mainimageView; // Widok obrazu.
	colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.clearValue.color = vk::ClearColorValue(color);



	vk::RenderingAttachmentInfoKHR depthAttachment = {};
	depthAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
	depthAttachment.imageView = swapchainFrames[imageIndex].depthBufferView; // Widok obrazu dla g³êbi.
	depthAttachment.imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	depthAttachment.clearValue.depthStencil.depth = 1.0f;
	depthAttachment.clearValue.depthStencil.stencil = 0.0f;

	// Debugging attachmentów g³êbi


	vk::RenderingInfoKHR renderingInfo = {};
	renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
	renderingInfo.renderArea.extent.width = swapchainExtent.width;
	renderingInfo.renderArea.extent.height = swapchainExtent.height;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &colorAttachment;
	renderingInfo.pDepthAttachment = &depthAttachment;



	auto& manager = MutexManager::getInstance();
	auto& taskmanager = TaskManager::getInstance();
	manager.lock("Descriptors");
	// Pipeline i layout
	vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("Unlit Pipeline");

	// Rozpoczêcie renderowania
	try {
		commandBuffer.beginRendering(&renderingInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
		return;
	}


	
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, swapchainFrames[imageIndex].postprocessDescriptorSet, nullptr);
	commandBuffer.pushConstants(pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vkRenderStructs::ProjectionData), &projection);
	prepare_scene(commandBuffer);
	
	uint32_t startInstance = 0;
	//Triangles

	for (const auto& [key, vector] : objects_to_rendering.pbr) {
		//std::cout << "Key: " << key << ", SceneObject count: " << vector.size() << "\n";



		render_objects(commandBuffer, pipelineInfo.pipelineLayout, key, startInstance, vector.size());
	}
	


	commandBuffer.endRendering();
	manager.unlock("Descriptors");
	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
}

void GraphicsEngine::record_pbr_draw_command(vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
	vk::CommandBufferInheritanceInfo info;
	info.sType = vk::StructureType::eCommandBufferInheritanceInfo;
	info.renderPass = nullptr;
	info.framebuffer = nullptr;
	info.subpass = 0;
	info.pNext = nullptr;

	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
	beginInfo.pInheritanceInfo = &info;

	try {
		commandBuffer.begin(beginInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin recording command buffer: " << err.what() << std::endl;
		return;
	}


	// Debugging dla kolorów i za³¹czników
	std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vk::RenderingAttachmentInfoKHR colorAttachment = {};
	colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
	colorAttachment.imageView = swapchainFrames[imageIndex].mainimageView; // Widok obrazu.
	colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.clearValue.color = vk::ClearColorValue(color);



	vk::RenderingAttachmentInfoKHR depthAttachment = {};
	depthAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
	depthAttachment.imageView = swapchainFrames[imageIndex].depthBufferView; // Widok obrazu dla g³êbi.
	depthAttachment.imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	depthAttachment.clearValue.depthStencil.depth = 1.0f;
	depthAttachment.clearValue.depthStencil.stencil = 0.0f;

	// Debugging attachmentów g³êbi


	vk::RenderingInfoKHR renderingInfo = {};
	renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
	renderingInfo.renderArea.extent.width = swapchainExtent.width;
	renderingInfo.renderArea.extent.height = swapchainExtent.height;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &colorAttachment;
	renderingInfo.pDepthAttachment = &depthAttachment;



	auto& manager = MutexManager::getInstance();
	auto& taskmanager = TaskManager::getInstance();
	manager.lock("Descriptors");
	// Pipeline i layout
	vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("Unlit Pipeline");
	;

	// Rozpoczêcie renderowania
	try {
		commandBuffer.beginRendering(&renderingInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
		return;
	}



	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, swapchainFrames[imageIndex].postprocessDescriptorSet, nullptr);
	commandBuffer.pushConstants(pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vkRenderStructs::ProjectionData), &projection);
	prepare_scene(commandBuffer);

	uint32_t startInstance = 0;
	//Triangles

	for (const auto& [key, vector] : objects_to_rendering.pbr) {
		//std::cout << "Key: " << key << ", SceneObject count: " << vector.size() << "\n";



		render_objects(commandBuffer, pipelineInfo.pipelineLayout, key, startInstance, vector.size());
	}



	commandBuffer.endRendering();
	manager.unlock("Descriptors");
	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
}

void GraphicsEngine::prepare_scene(vk::CommandBuffer commandBuffer) {
	vk::Buffer vertexBuffers[] = { vkResources::meshes->vertexBuffer.buffer };
	vk::DeviceSize offets[] = { 0 };
	commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offets);
	commandBuffer.bindIndexBuffer(vkResources::meshes->indexBuffer.buffer, 0, vk::IndexType::eUint32);

}

void GraphicsEngine::render_objects(vk::CommandBuffer commandBuffer,vk::PipelineLayout pipelineLayout ,uint64_t objectType, uint32_t& startInstance, uint32_t instanceCount) {
	
	
	
	int indexCount = vkResources::meshes->indexCounts.find(objectType)->second;
	
	int firstIndex = vkResources::meshes->firstIndices.find(objectType)->second;
	vkResources::atlasTextures->useTexture(commandBuffer, pipelineLayout);
	
	
	commandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, 0, startInstance);
	
	startInstance += instanceCount;
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

	auto& taskmanager = TaskManager::getInstance();
	/*
	taskmanager.submitTask(
		TaskPriority::DESCRIPTORS,
		[this](auto&&... args) { prepare_frame(std::forward<decltype(args)>(args)...); },
		imageIndex, scene, deltaTime, camera
	);
	*/
	prepare_frame(imageIndex, scene, deltaTime, camera);
	vk::CommandBuffer MainCommandBuffer = swapchainFrames[frameNumber].mainCommandBuffer;
	vk::CommandBuffer unlitCommandBuffer = swapchainFrames[frameNumber].unlitCommandBuffer;

	MainCommandBuffer.reset();
	unlitCommandBuffer.reset();

	taskmanager.submitTask(
		TaskPriority::RECORD_DRAW_COMMAND,
		[this](auto&&... args) { record_unlit_draw_command(std::forward<decltype(args)>(args)...); },
		unlitCommandBuffer, imageIndex
	);


	taskmanager.waitForPriorityTasks(TaskPriority::RECORD_DRAW_COMMAND);
	record_draw_command(MainCommandBuffer,unlitCommandBuffer,scene ,imageIndex);
	
	
	
	//taskmanager.waitForPriorityTasks(TaskPriority::DESCRIPTORS);
	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNumber].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &MainCommandBuffer;

	vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNumber].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;


	if (prefabsManager.getActiveWindowCount() > 0) {
		prefabsManager.update(deltaTime); // Aktualizujemy mened¿era

	}

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

	
	auto& taskmanager = TaskManager::getInstance();



	taskmanager.submitTask(
		TaskPriority::HIGH,
		[this](auto&&... args) { load_meshes_files(std::forward<decltype(args)>(args)...); }
	);
	taskmanager.submitTask(
		TaskPriority::HIGH,
		[this](auto&&... args) { load_textures_files(std::forward<decltype(args)>(args)...); }
	);
	taskmanager.submitTask(
		TaskPriority::HIGH,
		[this](auto&&... args) { load_scripts(std::forward<decltype(args)>(args)...); }
	);




	projection = vkRenderStructs::getProjectionMatrix(swapchainExtent);
	meshesManager = new vkMesh::MeshesManager(scene->root, scene->ecs);
	vkResources::meshes = new vkMesh::VertexMenagerie();
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 1;
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);

	iconDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(1), bindings);
	textureDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(1), bindings);
	UIFontTextureDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(1), bindings);
	cubemapDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(6)+1, bindings);
	


	vkImage::TextureInputChunk info;

	info.queue = graphicsQueue;
	info.commandBuffer = maincommandBuffer;
	info.logicalDevice = device;
	info.physicalDevice = physicalDevice;
	
	info.descriptorPool = textureDescriptorPool;
	info.layout = textureDescriptorSetLayout;
	info.filenames = nullptr;

	taskmanager.waitForPriorityTasks(TaskPriority::HIGH);
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	
	info.texturesNames = filesManager.getTexturesNames();
	vkResources::atlasTextures = new vkImage::Texture(info);


	std::vector<vkMesh::MeshLoader> test;
	for (std::string path : filesManager.getMeshesNames().fullPaths) {
		vkMesh::MeshLoader m(path.c_str());
		test.push_back(m);
	}
	
	size_t index = 0;
	for (vkMesh::MeshLoader m : test) {
		vkMesh::VertexBuffers buffer = m.getData();
		vkResources::meshes->consume(filesManager.getMeshesNames().hash[m.path], buffer.vertices, buffer.indicies);
	
	}


	
	vkMesh::FinalizationChunk finalizationInfo;
	finalizationInfo.logicalDevice = device;
	finalizationInfo.physicalDevice = physicalDevice;
	finalizationInfo.queue = transferQueue;
	finalizationInfo.commandBuffer = transferCommandBuffer;
	vkResources::meshes->finalize(finalizationInfo);


	info.descriptorPool = cubemapDescriptorPool;
	info.layout = cubemapDescriptorSetLayout;
	info.filenames = "resources/textures/cubemap.jpg";
	//cubemap->LoadCubemapData(info);

	info.descriptorPool = iconDescriptorPool;
	info.layout = iconDescriptorSetLayout;
	



	sceneEditor = new editor(scene, std::string(PROJECT_DIR), info, swapchainFormat, swapchainFrames[0].depthFormat);

	//fontManager = new UI::FontManager(physicalDevice, device, graphicsQueue, layout, descriptorPool, maincommandBuffer);
	testText = UImanager.create_text(glm::vec2(400, 400), glm::vec2(400, 400));
	fontManager = new UI::FontManager(physicalDevice,device, graphicsQueue, UIFontTextureDescriptorSetLayout, UIFontTextureDescriptorPool, maincommandBuffer);
}


void GraphicsEngine::prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera) {
	
	scene->update_objects_to_rendering(objects_to_rendering, scene->root);
	
	uiRenderingDrawData.reset();
	

	
	glm::vec3 eye = { 0.0f, 0.0f, -20.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);
	
	
	size_t i = 0;

	vkUtil::SwapChainFrame& _frame = swapchainFrames[imageIndex];

	

	UImanager.update(_frame.UIPositionSize, uiRenderingDrawData, _frame.UIFontPositionSize, fontManager);
	
	fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
	for (auto& [key, vector] : objects_to_rendering.unlit) {
		for (SceneObject* obj : vector) {
			_frame.modelsData[i].model = scene->ecs->getComponent<TransformComponent>(obj->id).get()->getTransform().getModelMatrix();
			_frame.modelsData[i].textureID = filesManager.getTexturesNames().getIndex(scene->ecs->getComponent<TextureComponent>(obj->id).get()->getColorTextureIndex());
			++i;
		}
	}
	for (auto& [key, vector] : objects_to_rendering.pbr) {
		for (SceneObject* obj : vector) {
			_frame.modelsData[i].model = scene->ecs->getComponent<TransformComponent>(obj->id).get()->getTransform().getModelMatrix();
			_frame.modelsData[i].textureID = filesManager.getTexturesNames().getIndex(scene->ecs->getComponent<TextureComponent>(obj->id).get()->getColorTextureIndex());
			++i;
		}
	}
	

	_frame.cameraData.view = view;//camera.GetViewMatrix();

	_frame.cameraData.camPos = glm::vec4(camera.Position, 1.0f);
	memcpy(_frame.cameraDataWriteLocation, &(_frame.cameraData), sizeof(vkUtil::CameraUBO));
	//if (i > 0) 
	memcpy(_frame.modelsDataWriteLocation, _frame.modelsData.data(), i * sizeof(vkUtil::MeshSBO)); 

	memcpy(_frame.UIPositionSizeDataWriteLocation, _frame.UIPositionSize.data(), uiRenderingDrawData.UIinstanceCount * sizeof(glm::vec4));
	memcpy(_frame.UIFontPositionSizeDataWriteLocation, _frame.UIFontPositionSize.data(), uiRenderingDrawData.UILettersinstanceCount * sizeof(vkUtil::FontSBO));
	
	//std::cout << _frame.UIPositionSize[0].x << " " << _frame.UIPositionSize[0].y << " " << _frame.UIPositionSize[0].z << " " << _frame.UIPositionSize[0].w << std::endl;

	_frame.write_postprocess_descriptors();
	_frame.write_UI_descriptors();
	


	

}

void GraphicsEngine::build_accelerationStructures() {
	
	vkAcceleration::AccelerationStructuresBuilderInfo info (device, vkResources::meshes);
	vkAcceleration::build_structures(info);
}

void GraphicsEngine::initial_cubemap() {
	//cubemap = new vkImage::CubemapEctTexture("resources/textures/cubemap.jpg");
}
