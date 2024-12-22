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

GraphicsEngine::GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode) {
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}


	make_instance();
	choice_device();

	vkResources::scenePipelines = new vkUtil::PipelineCache(device);
	vkResources::editorPipelines = new vkUtil::PipelineCache(device);


	create_descriptor_set_layouts();

	create_pipeline();

	finalize_setup(scene);
	make_assets(scene);


}

GraphicsEngine::~GraphicsEngine() {
	
	device.waitIdle();
	if (debugMode) {
		std::cout << "End!\n";
	}
	delete vkResources::scenePipelines;
	delete vkResources::editorPipelines;
	device.destroyRenderPass(imguiRenderPass);
	delete sceneEditor;
	delete vkResources::meshes;
	delete meshesManager;
	delete vkResources::atlasTextures;
	device.destroyCommandPool(CommandPool);
	device.destroyCommandPool(imguiCommandPool);

	device.destroyDescriptorSetLayout(iconDescriptorSetLayout);
	device.destroyDescriptorSetLayout(postprocessDescriptorSetLayout);
	device.destroyDescriptorSetLayout(textureDescriptorSetLayout);

	device.destroyDescriptorPool(postprocessDescriptorPool);
	device.destroyDescriptorPool(iconDescriptorPool);
	device.destroyDescriptorPool(imguiDescriptorPool);
	device.destroyDescriptorPool(textureDescriptorPool);
	
	
	cleanup_swapchain();
	
	device.destroy();

	instance.destroySurfaceKHR(surface);

	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	instance.destroy();
	glfwTerminate();
	
}

void GraphicsEngine::create_frame_resources(Scene* scene) {

	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	postprocessDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{
		

		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
		frame.computeFinished = vkInit::make_semaphore(device, debugMode);
		frame.inFlight = vkInit::make_fence(device, debugMode);
		frame.make_descriptors_resources(scene->getSceneObjectNumber(scene->root));
		frame.postprocessDescriptorSet = vkInit::allocate_descriptor_set(device, postprocessDescriptorPool, postprocessDescriptorSetLayout);
	}
}

void GraphicsEngine::create_pipeline() {

	vkUtil::PipelineCacheChunk pipeline;
	pipeline.type = 0;
	postprocessRenderPass = vkUtil::create_postprocess_renderpass(device, swapchainFormat, swapchainFrames[0].depthFormat);

	vkInit::PipelineBuilder pipelineBuilder(device);

	//Sky
	pipelineBuilder.set_overwrite_mode(true);
	pipelineBuilder.specify_vertex_format(
		vkMesh::getVertexInputBindingDescription(),
		vkMesh::getVertexInputAttributeDescription());
	pipelineBuilder.specify_vertex_shader("resources/shaders/vert.spv");
	pipelineBuilder.specify_fragment_shader("resources/shaders/frag.spv");
	pipelineBuilder.specify_swapchain_extent(swapchainExtent);
	//pipelineBuilder.set_renderpass(pipeline.renderPass);
	pipelineBuilder.clear_depth_attachment();
	pipelineBuilder.add_descriptor_set_layout(postprocessDescriptorSetLayout);
	pipelineBuilder.add_descriptor_set_layout(textureDescriptorSetLayout);
	pipelineBuilder.use_depth_test(true);
	pipelineBuilder.use_projection_matrix(true);
	pipelineBuilder.dynamicRendering = true;
	vkInit::GraphicsPipelineOutBundle output = pipelineBuilder.build(swapchainFormat,swapchainFrames[0].depthFormat);

	pipeline.pipelineLayout = output.layout;
	pipeline.pipeline = output.pipeline;

	vkResources::scenePipelines->addPipeline("Unlit Pipeline", pipeline);

	pipelineBuilder.reset();
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
}

void GraphicsEngine::create_frame_command_buffer() {
	
	CommandPool = vkInit::make_command_pool(physicalDevice, device, surface, debugMode);
	imguiCommandPool = vkInit::make_command_pool(physicalDevice, device, surface, debugMode);
	vkInit::commandBufferInputChunk commandBufferInput = { device,CommandPool ,imguiCommandPool, swapchainFrames };
	vkInit::commandBufferOutput output = vkInit::make_command_buffer(commandBufferInput, debugMode);
	maincommandBuffer = output.graphicCommandBuffer;
	vkInit::make_imgui_frame_command_buffers(commandBufferInput, debugMode);
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

	// Przyk³ad tworzenia deskryptora dla ImGui
	//ImGui_ImplVulkan_CreateFontsTexture(vkCommandBuffer);
	
}

void GraphicsEngine::finalize_setup(Scene* scene){
	imguiRenderPass = vkUtil::create_imgui_renderpass(device, swapchainFormat);
	create_framebuffers();
	create_frame_command_buffer();
	create_frame_resources(scene);
}

void GraphicsEngine::create_framebuffers(){
	
	vkInit::framebufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = imguiRenderPass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(frameBufferInput, swapchainFrames, debugMode);
	frameBufferInput.renderpass = postprocessRenderPass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_postprocess_framebuffer(frameBufferInput,swapchainFrames, debugMode);
	
}

void GraphicsEngine::load_scripts() {
	std::vector<std::string> ext = { ".cpp", };
	fileOperations::list_files_in_directory("\\core",fileOperations::cppNames, ext);
	scripts::compileAllScripts(fileOperations::cppNames, fileOperations::dllNames);
}

void GraphicsEngine::record_draw_command(vk::CommandBuffer commandBuffer,Scene* scene ,uint32_t imageIndex) {
	
	vk::CommandBufferBeginInfo beginInfo = {};

	// Sprawdzenie rozpoczêcia nagrywania command buffer
	try {
		commandBuffer.begin(beginInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin recording command buffer: " << err.what() << std::endl;
		return;
	}
	vk::ImageMemoryBarrier depthBarrier = {};
	depthBarrier.oldLayout = vk::ImageLayout::eUndefined; // obecny layout obrazu, np. undefined po stworzeniu
	depthBarrier.newLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal; // nowy layout
	depthBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	depthBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	depthBarrier.image = swapchainFrames[imageIndex].depthBuffer;
	depthBarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth; // zakres aspektu obrazu (kolor)
	depthBarrier.subresourceRange.baseMipLevel = 0;
	depthBarrier.subresourceRange.levelCount = 1;
	depthBarrier.subresourceRange.baseArrayLayer = 0;
	depthBarrier.subresourceRange.layerCount = 1;
	
	// Okreœl dostêp pamiêci, aby przejœæ z poprzedniego do nowego layoutu
	depthBarrier.srcAccessMask = {}; // Brak poprzednich operacji do synchronizacji
	depthBarrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite; // Docelowy dostêp, np. pisanie do attachmentu koloru

	// U¿yj vkCmdPipelineBarrier, aby zrealizowaæ barierê w command buffer
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTopOfPipe, // srcStageMask: najwczeœniejszy etap, brak poprzedniego u¿ycia
		vk::PipelineStageFlagBits::eEarlyFragmentTests, // dstStageMask: docelowy etap, w którym obraz bêdzie u¿ywany
		{}, // flagi bariery
		nullptr, nullptr, // brak barier pamiêciowych ani buforowych
		depthBarrier // wskaŸnik do bariery obrazu
	);





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

	
	for (const auto& [key, meshDataVector] : meshesManager->modelMatrices) {

		uint32_t k = 0;
		for (vkMesh::MeshManagerData data : meshDataVector) {
			if (data.sceneObject->isActive && scene->ecs->hasComponent<MeshComponent>(data.sceneObject->id))++k;
		}
		render_objects(commandBuffer, pipelineInfo.pipelineLayout,key, startInstance, k);
	}

	
	//commandBuffer.endRenderPass();

	commandBuffer.endRendering();

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


	sceneEditor->render_editor(commandBuffer, imguiRenderPass, swapchainFrames, meshesManager,swapchainExtent, imageIndex, debugMode);

	
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


	
	vk::CommandBuffer imgcommandBuffer = swapchainFrames[frameNumber].imguiCommandBuffer;
	vk::CommandBuffer sceneRenderCommandBuffer = swapchainFrames[frameNumber].commandBuffer;
	imgcommandBuffer.reset();
	sceneRenderCommandBuffer.reset();


	prepare_frame(imageIndex, scene, deltaTime, camera);
	//render_imgui(imgcommandBuffer,frameNumber,debugMode);
	record_draw_command(imgcommandBuffer,scene ,imageIndex);
	
	

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

	//TO DO NAPRAW TO ZJEBIE
	projection = vkRenderStructs::getProjectionMatrix(swapchainExtent);
	meshesManager = new vkMesh::MeshesManager(scene->root,scene->ecs);

	

	vkResources::meshes = new vkMesh::VertexMenagerie();
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 1;
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);

	iconDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(1), bindings);
	textureDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(1), bindings);
	vkImage::TextureInputChunk info;
	std::string str = std::string(PROJECT_DIR) + "\\core\\u.png";
	info.queue = graphicsQueue;
	info.commandBuffer = maincommandBuffer;
	info.logicalDevice = device;
	info.physicalDevice = physicalDevice;
	std::vector<std::string> ext = { ".png",".jpg" };
	
	fileOperations::list_files_in_directory("\\core", fileOperations::texturesNames,ext);
	
	
	
	info.descriptorPool = textureDescriptorPool;
	info.layout = textureDescriptorSetLayout;
	info.filenames = nullptr;
	info.texturesNames = fileOperations::texturesNames;
	vkResources::atlasTextures = new vkImage::Texture(info);
	ext[0] = ".obj";
	ext[1] = ".fbx";
	list_files_in_directory("\\core", fileOperations::meshesNames,ext);
	

	std::vector<vkMesh::MeshLoader> test;
	for (std::string path : fileOperations::meshesNames.fullPaths) {
		vkMesh::MeshLoader m(path.c_str());
		test.push_back(m);
	}
	
	size_t index = 0;
	for (vkMesh::MeshLoader m : test) {
		vkMesh::VertexBuffers buffer = m.getData();
		vkResources::meshes->consume(fileOperations::meshesNames.hash[m.path], buffer.vertices, buffer.indicies);
	
	}


	
	vkMesh::FinalizationChunk finalizationInfo;
	finalizationInfo.logicalDevice = device;
	finalizationInfo.physicalDevice = physicalDevice;
	finalizationInfo.queue = graphicsQueue;
	finalizationInfo.commandBuffer = maincommandBuffer;
	vkResources::meshes->finalize(finalizationInfo);
	//this->build_accelerationStructures();
	//info.texturesNames = texturesNames;
	info.filenames = str.c_str();
	info.descriptorPool = iconDescriptorPool;
	info.layout = iconDescriptorSetLayout;
	load_scripts();
	sceneEditor = new editor(scene, std::string(PROJECT_DIR), info, swapchainFormat, swapchainFrames[0].depthFormat);



}


void GraphicsEngine::prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera) {
	
	

	vkUtil::SwapChainFrame& _frame = swapchainFrames[imageIndex];
	glm::vec3 eye = { 0.0f, 0.0f, -20.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);
	_frame.cameraData.view = view;//camera.GetViewMatrix();

	_frame.cameraData.camPos = glm::vec4(camera.Position,1.0f);
	
	size_t i = 0;
	for (const auto& [key, meshDataVector] : meshesManager->modelMatrices) {
		for (const auto& meshData : meshDataVector) {
			if (meshData.modelMatrix && meshData.sceneObject) {  // Sprawdzamy, czy wskaŸnik jest wa¿ny
				if (meshData.sceneObject->isActive && scene->ecs->hasComponent<MeshComponent>(meshData.sceneObject->id)){


					_frame.modelsData[i].model = *meshData.modelMatrix;//*meshData.modelMatrix;

					TextureComponent* textureComponent = scene->ecs->getComponent<TextureComponent>(meshData.sceneObject->id).get();
					
					if (textureComponent != nullptr) {
						_frame.modelsData[i].textureID = fileOperations::texturesNames.getIndex(textureComponent->getColorTextureIndex());
					}
					else {
						_frame.modelsData[i].textureID = 0;
						
					}
					i++;
				}				
			}
		}
	}
	/*
	std::cout << "Jestem macierzem obiektu 0:" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << _frame.modelsData[1].model[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << "Jestem macierzem obiektu 1:" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << _frame.modelsData[1].model[i][j] <<"  ";
		}
		std::cout << std::endl;
	}
	std::cout << "Jestem macierzem obiektu 2:" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << _frame.modelsData[2].model[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	*/
	
	memcpy(_frame.cameraDataWriteLocation, &(_frame.cameraData), sizeof(vkUtil::CameraUBO));
	memcpy(_frame.modelsDataWriteLocation, _frame.modelsData.data(), i * sizeof(vkUtil::MeshSBO));
	_frame.write_postprocess_descriptors();

}

void GraphicsEngine::build_accelerationStructures() {
	
	vkAcceleration::AccelerationStructuresBuilderInfo info (device, vkResources::meshes);
	vkAcceleration::build_structures(info);
}
