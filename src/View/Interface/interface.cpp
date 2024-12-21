#include "View/Interface/interface.h"


#include <Scene/ECS/components/componentFabric.h>
#include "Scene/ECS/scripts/scriptCompiler.h"

#include "fileOperations/resources.h"




void editor::create_miniatures(vk::PhysicalDevice physicalDevice,
	vk::Device device,
	vk::CommandBuffer commandBuffer,
	vk::Queue queue, 
	vk::Format pictureFormat,
	vk::Format depthFormat) {
	//vk::Device logicalDevice;
	//vk::PhysicalDevice physicalDevice;
	//vk::CommandBuffer commandBuffer;
	//vk::Queue queue;
	//vkImage::TexturesNames textures;
	//vk::Format pictureFormat;
	//vk::Format depthFormat;
	//int number_of_models;

	vkThumbs::ThumbsManagerInput input;
	input.physicalDevice = physicalDevice;
	input.logicalDevice = device;
	input.commandBuffer = commandBuffer;
	input.queue = queue;
	input.pictureFormat = pictureFormat;
	input.depthFormat = depthFormat;


	miniatureManager = new vkThumbs::ThumbsManager(input);
}

editor::editor(Scene* scene,std::string path, vkImage::TextureInputChunk info,
	vk::Format pictureFormat,
	vk::Format depthFormat){
	this->scene = scene;

	this->filesExploresData.baseFolder = path;
	this->filesExploresData.currentFolder = filesExploresData.baseFolder;

	texture = new vkImage::Texture(info);
	create_miniatures(info.physicalDevice,info.logicalDevice,info.commandBuffer,info.queue, pictureFormat, depthFormat);
	
}
editor::~editor() {
	delete texture;
	delete miniatureManager;
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


}
void editor::render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, vkMesh::MeshesManager* meshesManager,vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode){

	// Renderowanie ImGui
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();
	ImGui::NewFrame();

	/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (ImGui::Button("Click Me")) {
		// Kod do wykonania po naciœniêciu przycisku
		std::cout << "Button was clicked!" << std::endl;
		scripts::compileAllScripts(scriptsFiles.cpp,scriptsFiles.dll,scriptsFiles.scriptsCounter);
	}
	render_file_explorer();

	// Twoje GUI
	ImGui::Begin("Hello, ImGui!");
	ImGui::Text("This is a Vulkan window with ImGui!");
	size_t i = 0;
	DisplaySceneObject(scene->root);
	render_components_gui(models, textures, meshesManager);
	ImGui::End();
	*/ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Wysokoœæ i szerokoœæ ekranu
	

	vkImGui::render_editor(miniatureManager, filesExploresData, scene->root,selectedObject,selectedComponentType,scene->ecs,meshesManager);
	
	
	
	
	vk::RenderPassBeginInfo imguiRenderpassInfo = {};
	imguiRenderpassInfo.renderPass = imguiRenderPass;
	imguiRenderpassInfo.framebuffer = swapchainFrames[numberOfFrame].imguiFrameBuffer;
	imguiRenderpassInfo.renderArea.offset.x = 0;
	imguiRenderpassInfo.renderArea.offset.y = 0;
	imguiRenderpassInfo.renderArea.extent = swapchainExtent;
	//vk::ClearValue clearColor = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));
	//imguiRenderpassInfo.clearValueCount = 1;
	//imguiRenderpassInfo.pClearValues = &clearColor;

	// Rozpocznij render pass
	commandBuffer.beginRenderPass(imguiRenderpassInfo, vk::SubpassContents::eInline);

	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	// Zakoñcz render pass
	commandBuffer.endRenderPass();
	/*
	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}*/
}










