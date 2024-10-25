#include "View/Interface/interface.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <Scene/ECS/components/transformComponent.h>
#include <Scene/ECS/components/meshComponent.h>
#include <Scene/ECS/components/componentFabric.h>




editor::editor(Scene* scene){
	this->scene = scene;
}

void editor::render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames,modelNames models ,vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode){
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
		ImGui::Separator();
		ImGui::Text("Components:");
		auto components = scene->ecs->getAllComponents(selectedObject->id);
		
		for (const auto& componentPtr : components) {
			Component* comp = componentPtr.get();  // Surowy wskaŸnik

			std::string label = comp->getLabel();
			

			if (ImGui::TreeNode(label.c_str())) {
				// Wyœwietlanie specyficznych pól dla komponentów
				if (comp->getType() == ComponentType::Transform) {
					TransformComponent* transform = dynamic_cast<TransformComponent*>(comp);
					if (transform != nullptr) {
						// Wyœwietlanie i edytowanie transformacji
						Transform& transformData = transform->getModifyableTransform();
						ImGui::DragFloat3("Position", &transformData.getModifyableLocalPosition().x, 0.1f);
						ImGui::DragFloat3("Rotation", &transformData.getModifyableLocalRotation().x, 0.1f);
						ImGui::DragFloat3("Scale", &transformData.getModifyableLocalScale().x, 0.1f);
					}
					ImGui::SameLine;
					this->RemoveComponent(scene->ecs,selectedObject->id,transform);
				}
				else if (comp->getType() == ComponentType::Mesh) {
					static char searchQuery[128] = "";
					meshComponent* mesh = dynamic_cast<meshComponent*>(comp);

					// Wyœwietl aktualnie przypisany model jako rozwijan¹ listê
					std::string currentModel;
					if (mesh->getIndex() >= 0)
						currentModel = models.fileNames[mesh->getIndex()];
					else currentModel = "";

					if (ImGui::BeginCombo("##modelSelector", currentModel.c_str())) { // Combo bez etykiety z lewej strony
						// Pole wyszukiwania w rozwijanej liœcie
						ImGui::InputText("Search Models", searchQuery, IM_ARRAYSIZE(searchQuery));

						// Pêtla do wyœwietlania wyników wyszukiwania
						for (size_t i = 0; i < models.fileNames.size(); ++i) {
							const std::string& modelName = models.fileNames[i];
							if (modelName.find(searchQuery) != std::string::npos) {

								ImGui::PushID(i);

								// Po³¹czony tekst nazwy modelu i œcie¿ki
								std::string displayText = modelName + "\nPath: " + models.fullPaths[i];

								// Wybór modelu
								if (ImGui::Selectable(displayText.c_str(), mesh->getIndex() == i)) {
									mesh->setIndex(i);  // Zapisz wybrany indeks
									std::cout << "Wybrano indeks: " << mesh->getIndex() << std::endl;
								}

								ImGui::PopID();
							}
						}
						ImGui::EndCombo();
					}

					// Przycisk do usuwania komponentu obok selektora
					ImGui::SameLine();
					if (ImGui::Button("Remove")) {
						this->RemoveComponent(scene->ecs, selectedObject->id, mesh);
					}
				}
				// Zakoñcz wêze³ drzewa
				ImGui::TreePop();
			}
		}
		/////////////////////////////////////////////////////////ADD component
		
		AddComponent(scene->ecs, selectedObject->id);
		
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

	//ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Przyk³adowo, wiêksza czcionka
	if (ImGui::TreeNode(obj->getName().c_str())) {
		if (ImGui::IsItemClicked()) {
			selectedObject = obj;
		}


		ImGui::SameLine();
		if (ImGui::Button("Add Child")) {
			AddSceneObject(obj);
		}

		if (obj->parent != nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Remove")) {

				RemoveSceneObject(obj);
				ImGui::TreePop();
				//ImGui::PopFont(); // Przywróæ domyœlny rozmiar czcionki
				return;
			}
		}
		

		// Rekurencyjne rysowanie dzieci obiektu
		for (auto& child : obj->children) {
			DisplaySceneObject(child);
		}

		ImGui::TreePop();
	}
	//ImGui::PopFont(); // Przywróæ domyœlny rozmiar czcionki (jeœli zmienia³eœ)
}


void editor::RemoveSceneObject(SceneObject* obj) {
	// Usuwanie dzieci rekurencyjnie

	while (!obj->children.empty()) {
		std::cout << obj->children.size() << std::endl;
		SceneObject* child = obj->children.back();
		obj->children.pop_back();  // Usuñ wskaŸnik z listy dzieci
		RemoveSceneObject(child);  // Usuñ dziecko rekurencyjnie
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

void editor::AddComponent(ecs::ECS* ecs,ecs::Entity entity) {
	// Wyœwietlenie przycisku do dodania komponentu
	if (ImGui::Button("Add Component")) {
		// Prze³¹cz widocznoœæ selektora komponentów
		showComponentSelector = !showComponentSelector; // Zmiana stanu
	}

	// Jeœli selektor komponentów ma byæ widoczny
	if (showComponentSelector) {
		// Lista typów komponentów do wyboru (pomijamy None)
		std::vector<const char*> componentNames = {
			"Transform",
			"Mesh",
			"Render",
			"Physics",
			"Script"
			// Dodaj inne typy komponentów
		};

		// Tworzymy rozwijane menu do wyboru komponentu
		int currentIndex = static_cast<int>(selectedComponentType);
		if (ImGui::Combo("Select Component", &currentIndex, componentNames.data(), componentNames.size())) {
			selectedComponentType = static_cast<ComponentType>(currentIndex);
			// Tutaj mo¿na dodaæ logikê do tworzenia nowego komponentu na podstawie selectedComponentType
			// Na przyk³ad:
			// auto component = CreateComponent(selectedComponentType);
			std::cout << currentIndex << std::endl;
		}

		// Przycisk do zamkniêcia selektora
		if (ImGui::Button("Add",ImVec2(100,0))) {

			showComponentSelector = false; // Ukryj selektor po naciœniêciu
			ecs->addComponent(entity, createComponent(static_cast<ComponentType>(currentIndex)));

		}
		ImGui::SameLine();
		if (ImGui::Button("Close", ImVec2(100, 0))) {
			showComponentSelector = false; // Ukryj selektor po naciœniêciu
		}
	}


}
void editor::RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component) {
	if (ImGui::Button("Remove this Component")) {
		// Kod do wykonania po naciœniêciu przycisku
		ecs->removeComponent(component, entity);
	}
}

void editor::AddSceneObject(SceneObject* obj) {
	SceneObject* newChild = new SceneObject(scene->ecs);
	obj->addChild(newChild); // Dodaj nowy obiekt do dzieci
}