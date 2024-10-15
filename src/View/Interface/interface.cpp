#include "View/Interface/interface.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>




editor::editor(Scene* scene){
	this->scene = scene;
}

void editor::render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode){
	vk::CommandBufferBeginInfo beginInfo{};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.pInheritanceInfo = nullptr;  // Opcjonalnie


	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording command buffer!" << std::endl;
		}
	}

	// Renderowanie ImGui
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();
	ImGui::NewFrame();
	if (ImGui::Button("Click Me")) {
		// Kod do wykonania po naciœniêciu przycisku
		std::cout << "Button was clicked!" << std::endl;
	}

	// Twoje GUI
	ImGui::Begin("Hello, ImGui!");
	ImGui::Text("This is a Vulkan window with ImGui!");
	size_t i = 0;
	DisplaySceneObject(scene->root);
	if (selectedObject != nullptr) {
		ImGui::Begin("Selected Object Properties");
		ImGui::Text("Object Name: %s", selectedObject->getName().c_str());

		

		ImGui::End();
	}
	ImGui::End();
	vk::RenderPassBeginInfo imguiRenderpassInfo = {};
	imguiRenderpassInfo.renderPass = imguiRenderPass;
	imguiRenderpassInfo.framebuffer = swapchainFrames[numberOfFrame].imguiFrameBuffer;
	imguiRenderpassInfo.renderArea.offset.x = 0;
	imguiRenderpassInfo.renderArea.offset.y = 0;
	imguiRenderpassInfo.renderArea.extent = swapchainExtent;
	vk::ClearValue clearColor = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));
	imguiRenderpassInfo.clearValueCount = 1;
	imguiRenderpassInfo.pClearValues = &clearColor;

	// Rozpocznij render pass
	commandBuffer.beginRenderPass(imguiRenderpassInfo, vk::SubpassContents::eInline);

	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	// Zakoñcz render pass
	commandBuffer.endRenderPass();

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
}

void editor::DisplaySceneObject(SceneObject* obj) {
	if (ImGui::TreeNode(obj->getName().c_str())) {
		if (ImGui::IsItemClicked()) {
			selectedObject = obj;
		}

		if (obj->parent != nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Remove")) {

				RemoveSceneObject(obj);
				ImGui::TreePop();
				return;
			}
		}
		

		// Rekurencyjne rysowanie dzieci obiektu
		for (auto& child : obj->children) {
			DisplaySceneObject(child);
		}

		ImGui::TreePop();
	}
}


void editor::RemoveSceneObject(SceneObject* obj) {
	// Usuwanie dzieci rekurencyjnie
	std::cout << "Kabe" << std::endl;
	while (!obj->children.empty()) {
		std::cout << obj->children.size() << std::endl;
		SceneObject* child = obj->children.back();
		obj->children.pop_back();  // Usuñ wskaŸnik z listy dzieci
		RemoveSceneObject(child);  // Usuñ dziecko rekurencyjnie
		std::cout << "Kabe" << std::endl;
	}

	// Usuñ obiekt z rodzica, jeœli istnieje
	if (obj->parent != nullptr) {
		auto& siblings = obj->parent->children;
		std::cout << siblings.size()<<std::endl;
		// ZnajdŸ i usuñ wskaŸnik z listy dzieci rodzica
		auto it = std::find(siblings.begin(), siblings.end(), obj);
		if (it != siblings.end()) {
			// Usuñ wskaŸnik z wektora dzieci rodzica
			siblings.erase(it);
			std::cout << siblings.size() << std::endl;
		}
	}

	// Zwolnij pamiêæ obiektu
	delete obj;
	selectedObject = nullptr;
}
