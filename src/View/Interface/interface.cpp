#include "View/Interface/interface.h"


#include <Scene/ECS/components/componentFabric.h>
#include "Scene/ECS/scripts/scriptCompiler.h"
#include "../thirdParty/ImPlot/implot.h"
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

	create_miniatures(info.physicalDevice,info.logicalDevice,info.commandBuffer,info.queue, pictureFormat, depthFormat);
	//fft = new math::FastFourierTransform();
}
editor::~editor() {
	//delete fft;
	delete miniatureManager;
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();


}
void editor::render_editor(vk::CommandBuffer commandBuffer, std::vector<vkUtil::SwapChainFrame>& swapchainFrames, RenderObjects* objects, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode, float dt) {

	// Renderowanie ImGui
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();
	ImGui::NewFrame();
	//fft->update(dt);
	/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (ImGui::Button("Click Me")) {
		// Kod do wykonania po naci�ni�ciu przycisku
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

	// Wysoko�� i szeroko�� ekranu


	vkImGui::render_editor(miniatureManager, filesExploresData, scene->root, selectedObject, selectedComponentType, scene->ecs, objects);



	vk::RenderingAttachmentInfoKHR colorAttachment = {};
	colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfoKHR;
	colorAttachment.imageView = swapchainFrames[numberOfFrame].mainimageView; // Widok obrazu.
	colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	//colorAttachment.clearValue.color = vk::ClearColorValue(color);




	vk::RenderingInfoKHR renderingInfo = {};
	renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
	renderingInfo.renderArea.extent.width = swapchainExtent.width;
	renderingInfo.renderArea.extent.height = swapchainExtent.height;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &colorAttachment;
	renderingInfo.pDepthAttachment = nullptr;




	// Rozpocz�cie renderowania
	try {
		commandBuffer.beginRendering(&renderingInfo);

	}
	catch (vk::SystemError err) {
		std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
		return;
	}
	// Inicjalizacja generatora liczb losowych
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Losowe dane dla wykres�w
	
		// Kod do wy�wietlenia wykres�w
	/*
	ImGui::Begin("Wave Simulation 3D");

	if (ImPlot::BeginPlot("3D Wave", ImVec2(-1, -1))) {
		for (int x = 0; x < fft->gridSize; ++x) {
			for (int y = 0; y < fft->gridSize; ++y) {
				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				float fz = fft->heightMap[x][y];

				ImPlot::PlotScatter("Wave", &fx, &fy, &fz, 1);
			}
		}
		ImPlot::EndPlot();
	}
	ImGui::End();
	*/
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);


		commandBuffer.endRendering();

		vk::ImageMemoryBarrier barrier = {};
		barrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal; // obecny layout obrazu, np. undefined po stworzeniu
		barrier.newLayout = vk::ImageLayout::ePresentSrcKHR; // nowy layout
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = swapchainFrames[numberOfFrame].mainimage;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor; // zakres aspektu obrazu (kolor)
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;


		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead;

		// U�yj vkCmdPipelineBarrier, aby zrealizowa� barier� w command buffer
		commandBuffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eTopOfPipe, // srcStageMask: najwcze�niejszy etap, brak poprzedniego u�ycia
			vk::PipelineStageFlagBits::eColorAttachmentOutput, // dstStageMask: docelowy etap, w kt�rym obraz b�dzie u�ywany
			{}, // flagi bariery
			nullptr, nullptr, // brak barier pami�ciowych ani buforowych
			barrier // wska�nik do bariery obrazu
		);


	}






