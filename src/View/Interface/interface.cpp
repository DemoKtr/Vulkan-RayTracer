#include "View/Interface/interface.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <Scene/ECS/components/componentFabric.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>




void editor::create_miniatures(vk::PhysicalDevice physicalDevice,
	vk::Device device,
	vk::CommandBuffer commandBuffer,
	vk::Queue queue, 
	vkImage::TexturesNames textures,
	vkMesh::VertexMenagerie* meshes,
	vk::Format pictureFormat,
	vk::Format depthFormat,
	int modelsNumber
	) {
	//vk::Device logicalDevice;
	//vk::PhysicalDevice physicalDevice;
	//vk::CommandBuffer commandBuffer;
	//vk::Queue queue;
	//vkImage::TexturesNames textures;
	//vkMesh::VertexMenagerie* meshes;
	//vk::Format pictureFormat;
	//vk::Format depthFormat;
	//int number_of_models;

	vkThumbs::ThumbsManagerInput input;
	input.physicalDevice = physicalDevice;
	input.logicalDevice = device;
	input.commandBuffer = commandBuffer;
	input.queue = queue;
	input.textures = textures;
	input.meshes = meshes;
	input.pictureFormat = pictureFormat;
	input.depthFormat = depthFormat;
	input.number_of_models = modelsNumber;
	miniatureManager = new vkThumbs::ThumbsManager(input);
}

editor::editor(Scene* scene,std::string path, vkImage::TextureInputChunk info, vkImage::TexturesNames textures, vkMesh::VertexMenagerie* meshes,
	vk::Format pictureFormat,
	vk::Format depthFormat,
	int modelsNumber){
	this->scene = scene;
	
	baseFolder = path;
	currentFolder = baseFolder;
	
	texture = new vkImage::Texture(info);
	
	create_miniatures(info.physicalDevice,info.logicalDevice,info.commandBuffer,info.queue, textures,meshes, pictureFormat, depthFormat,modelsNumber);
}
editor::~editor() {
	delete texture;
	delete miniatureManager;
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


}
void editor::render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames,modelNames models, vkImage::TexturesNames textures, vkMesh::MeshesManager* meshesManager,vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode){

	// Renderowanie ImGui
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplVulkan_NewFrame();
	ImGui::NewFrame();
	if (ImGui::Button("Click Me")) {
		// Kod do wykonania po naciœniêciu przycisku
		std::cout << "Button was clicked!" << std::endl;
	}
	render_file_explorer();

	// Twoje GUI
	ImGui::Begin("Hello, ImGui!");
	ImGui::Text("This is a Vulkan window with ImGui!");
	size_t i = 0;
	DisplaySceneObject(scene->root);
	render_components_gui(models, textures, meshesManager);
	ImGui::End();
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

void editor::rmb_click_render(std::filesystem::path path){
	
	
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("ContextMenu");
		std::cout << "1" << std::endl;
	}


		if (ImGui::BeginPopup("ContextMenu")) {
			std::cout << "2" << std::endl;
			if (ImGui::MenuItem("Copy")) {
				clipboard = path.string();
				isCut = false; // Ustaw flagê na kopiowanie
			}
			if (ImGui::MenuItem("Cut")) {
				clipboard = path.string();
				isCut = true; // Ustaw flagê na wycinanie
			}
			if (ImGui::MenuItem("Delete")) {
				std::filesystem::remove(path);
			}
			if (ImGui::MenuItem("Paste")) {
				if (!clipboard.empty()) {
					std::filesystem::path destination = currentFolder / std::filesystem::path(clipboard).filename();

					try {
						if (isCut) {
							std::filesystem::rename(clipboard, destination); // Przenieœ plik
						}
						else {
							std::filesystem::copy(clipboard, destination, std::filesystem::copy_options::overwrite_existing); // Skopiuj plik
						}
					}
					catch (const std::filesystem::filesystem_error& e) {
						std::cerr << "B³¹d operacji: " << e.what() << std::endl;
					}

					if (isCut) {
						clipboard.clear(); // Wyczyœæ schowek po wyciêciu
					}
				}
			}
			ImGui::EndPopup();
		}

	

}

void editor::render_components_gui(modelNames models, vkImage::TexturesNames textures, vkMesh::MeshesManager* meshesManager) {
	if (selectedObject != nullptr) {
		ImGui::Begin("Selected Object Properties");
		ImGui::Text("Object Name: %s", selectedObject->getName().c_str());
		ImGui::Separator();
		ImGui::Text("Components:");
		auto components = scene->ecs->getAllComponents(selectedObject->id);
		ImGui::Checkbox("Active: ", &selectedObject->isActive);
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
						glm::vec3 eulerAngles = glm::eulerAngles(transformData.getModifyableLocalRotation());
						ImGui::DragFloat3("Position", &transformData.getModifyableLocalPosition().x, 0.1f);
						if (ImGui::DragFloat3("Rotation", &eulerAngles.x, 0.1f)) {
							glm::quat newRotation = glm::yawPitchRoll(eulerAngles.y, eulerAngles.x, eulerAngles.z);
							transformData.setLocalRotation(newRotation);
							// Przekszta³cenie k¹tów Eulerów do kwaternionu tylko po zmianie wartoœci
							transformData.setLocalRotation(glm::quat(eulerAngles));
						}

						ImGui::DragFloat3("Scale", &transformData.getModifyableLocalScale().x, 0.1f);
						transformData.computeModelMatrix();
					}
					ImGui::SameLine;
					//this->RemoveComponent(scene->ecs, selectedObject->id, transform);
				}
				else if (comp->getType() == ComponentType::Mesh) {
					static char searchQuery[128] = "";
					MeshComponent* mesh = dynamic_cast<MeshComponent*>(comp);

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
								VkDescriptorSet qqq = texture->getDescriptorSet();
								ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
								ImVec2 imageSize(64, 64);
								ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
								// Wybór modelu
								
								ImGui::Image(miniatureManager->get_mesh_icon(i), imageSize);
								
								ImGui::SameLine;
								if (ImGui::Selectable(displayText.c_str(), mesh->getIndex() == i)) {
									meshesManager->updateMeshIndex(selectedObject, i, scene->ecs);
									mesh->setIndex(i);  // Zapisz wybrany indeks
									
									std::cout << "Wybrano indeks: " << mesh->getIndex() << std::endl;
								}

								ImGui::PopID();
							}
						}
						ImGui::EndCombo();
					}

					// Przycisk do usuwania komponentu obok selektora


					this->RemoveComponent(scene->ecs, selectedObject->id, mesh);

				}
				/**/
				else if (comp->getType() == ComponentType::Texture) {
					static char searchQuery[128] = "";
					TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(comp);

					ImGui::Checkbox("Is PBR Texture", textureComponent->isPBRTexture());

					// Wyœwietl aktualnie przypisany model jako rozwijan¹ listê
					std::string currentTexture;
					if (textureComponent->getColorTextureIndex() >= 0)
						currentTexture = textures.fileNames[textureComponent->getColorTextureIndex()];
					else currentTexture = "";

					if (ImGui::BeginCombo("##textureSelector", currentTexture.c_str())) { // Combo bez etykiety z lewej strony
						// Pole wyszukiwania w rozwijanej liœcie
						ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

						// Pêtla do wyœwietlania wyników wyszukiwania
						for (size_t i = 0; i < textures.fileNames.size(); ++i) {
							const std::string& textureName = textures.fileNames[i];
							if (textureName.find(searchQuery) != std::string::npos) {

								ImGui::PushID(i);

								// Po³¹czony tekst nazwy modelu i œcie¿ki
								std::string displayText = textureName + "\nPath: " + textures.fullPaths[i];


								VkDescriptorSet qqq = texture->getDescriptorSet();
								ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
								ImVec2 imageSize(64, 64);
								ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
								// Wybór modelu
								ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
								ImGui::SameLine;
								if (ImGui::Selectable(displayText.c_str(), textureComponent->getColorTextureIndex() == i)) {
									textureComponent->setColorTextureIndex(i);  // Zapisz wybrany indeks
									std::cout << "Wybrano indeks: " << textureComponent->getColorTextureIndex() << std::endl;
								}

								ImGui::PopID();
							}
						}
						ImGui::EndCombo();
					}
					if (*textureComponent->isPBRTexture()) {
						std::string currentNormalTexture;
						if (textureComponent->getNormalTextureIndex() >= 0)
							currentNormalTexture = textures.fileNames[textureComponent->getNormalTextureIndex()];
						else currentNormalTexture = "";

						if (ImGui::BeginCombo("##normalSelector", currentNormalTexture.c_str())) { // Combo bez etykiety z lewej strony
							// Pole wyszukiwania w rozwijanej liœcie
							ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

							// Pêtla do wyœwietlania wyników wyszukiwania
							for (size_t i = 0; i < textures.fileNames.size(); ++i) {
								const std::string& textureName = textures.fileNames[i];
								if (textureName.find(searchQuery) != std::string::npos) {

									ImGui::PushID(i);

									// Po³¹czony tekst nazwy modelu i œcie¿ki
									std::string displayText = textureName + "\nPath: " + textures.fullPaths[i];

									VkDescriptorSet qqq = texture->getDescriptorSet();
									ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
									ImVec2 imageSize(64, 64);
									ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
									// Wybór modelu
									ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
									ImGui::SameLine;
									if (ImGui::Selectable(displayText.c_str(), textureComponent->getNormalTextureIndex() == i)) {
										textureComponent->setNormalTextureIndex(i);  // Zapisz wybrany indeks
										std::cout << "Wybrano indeks: " << textureComponent->getNormalTextureIndex() << std::endl;
									}

									ImGui::PopID();
								}
							}
							ImGui::EndCombo();
						}
							std::string currentARMTexture;
							if (textureComponent->getARMTextureIndex() >= 0)
								currentARMTexture = textures.fileNames[textureComponent->getARMTextureIndex()];
							else currentARMTexture = "";

							if (ImGui::BeginCombo("##ARMtextureSelector", currentARMTexture.c_str())) { // Combo bez etykiety z lewej strony
								// Pole wyszukiwania w rozwijanej liœcie
								ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

								// Pêtla do wyœwietlania wyników wyszukiwania
								for (size_t i = 0; i < textures.fileNames.size(); ++i) {
									const std::string& textureName = textures.fileNames[i];
									if (textureName.find(searchQuery) != std::string::npos) {

										ImGui::PushID(i);

										// Po³¹czony tekst nazwy modelu i œcie¿ki
										std::string displayText = textureName + "\nPath: " + textures.fullPaths[i];

										VkDescriptorSet qqq = texture->getDescriptorSet();
										ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
										ImVec2 imageSize(64, 64);
										ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
										// Wybór modelu
										ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
										ImGui::SameLine;
										if (ImGui::Selectable(displayText.c_str(), textureComponent->getARMTextureIndex() == i)) {
											textureComponent->setARMTextureIndex(i);  // Zapisz wybrany indeks
											std::cout << "Wybrano indeks: " << textureComponent->getARMTextureIndex() << std::endl;
										}

										ImGui::PopID();
									}
								}
								ImGui::EndCombo();
							}
						std::string currentDepthTexture;
						if (textureComponent->getDepthTextureIndex() >= 0)
							currentDepthTexture = textures.fileNames[textureComponent->getDepthTextureIndex()];
						else currentDepthTexture = "";

						if (ImGui::BeginCombo("##DepthtextureSelector", currentDepthTexture.c_str())) { // Combo bez etykiety z lewej strony
									// Pole wyszukiwania w rozwijanej liœcie
							ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

									// Pêtla do wyœwietlania wyników wyszukiwania
							for (size_t i = 0; i < textures.fileNames.size(); ++i) {
								const std::string& textureName = textures.fileNames[i];
								if (textureName.find(searchQuery) != std::string::npos) {

									ImGui::PushID(i);

											// Po³¹czony tekst nazwy modelu i œcie¿ki
									std::string displayText = textureName + "\nPath: " + textures.fullPaths[i];

									VkDescriptorSet qqq = texture->getDescriptorSet();
									ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
									ImVec2 imageSize(64, 64);
									ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
									// Wybór modelu
									ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
									ImGui::SameLine;
									if (ImGui::Selectable(displayText.c_str(), textureComponent->getDepthTextureIndex() == i)) {
										textureComponent->setDepthTextureIndex(i);  // Zapisz wybrany indeks
										std::cout << "Wybrano indeks: " << textureComponent->getDepthTextureIndex() << std::endl;
									}

									ImGui::PopID();
								}
							}
								ImGui::EndCombo();
						}



								

					}
							// Zakoñcz wêze³ drzewa
					this->RemoveComponent(scene->ecs, selectedObject->id, textureComponent);
				}
				ImGui::TreePop();
			}
		
		}
	AddComponent(scene->ecs, selectedObject->id);
	ImGui::End();
	
	}
			
}

void editor::render_file_explorer() {
	if (ImGui::Begin("File Browser")) {
		if (currentFolder != baseFolder) {
			if (ImGui::Button("..")) {
				auto parentFolder = currentFolder.parent_path();
				if (parentFolder.string().find(baseFolder.string()) == 0) {
					currentFolder = parentFolder;
				}
			}
		}

		for (const auto& entry : std::filesystem::directory_iterator(currentFolder)) {

			const auto& path = entry.path();
			std::string name = path.filename().string();

			
			
			VkDescriptorSet qqq = texture->getDescriptorSet();
			ImTextureID imguiTextureId = reinterpret_cast<ImTextureID>(qqq);
			ImVec2 imageSize(64, 64);
			ImVec2 selectableSize(imageSize.x + 100, imageSize.y);
			
			ImGui::PushID(name.c_str());
			ImGui::Image(imguiTextureId, imageSize);
			if (ImGui::Selectable((name + "/").c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
				if (entry.is_directory()) {
					currentFolder = path;
					ImGui::SameLine();
					ImGui::Text(name.c_str());

				}
				else {
					std::string command;
#ifdef _WIN32
					command = "start \"\" \"" + path.string() + "\"";
#elif __APPLE__
					command = "open \"" + path.string() + "\"";
#else
					command = "xdg-open \"" + path.string() + "\"";
#endif
					std::system(command.c_str());
					ImGui::SameLine();
					ImGui::Text(name.c_str());

				}
			}
			
			

			rmb_click_render(path);

			
			

			// Obs³uga przeci¹gania
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				DragDropData data;
				strncpy(data.fullPath, path.string().c_str(), sizeof(data.fullPath) - 1);
				strncpy(data.name, name.c_str(), sizeof(data.name) - 1);

				ImGui::SetDragDropPayload("FILE_PATH", &data, sizeof(data));
				ImGui::Text("Move: %s", data.name);
				ImGui::EndDragDropSource();
			}

			// Obs³uga upuszczania
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH")) {
					const DragDropData* droppedData = static_cast<const DragDropData*>(payload->Data);

					std::string fullPath(droppedData->fullPath);
					std::string droppedName(droppedData->name);

					std::cout << "Przenoszenie pliku: " << fullPath << std::endl;

					if (!fullPath.empty()) {
						try {
							std::filesystem::path sourcePath(fullPath);
							std::string filename = sourcePath.filename().string();
							std::filesystem::path destination = currentFolder / name / droppedName;



							std::filesystem::create_directories(destination );
							std::cout << "Destination: " << destination << "!!!" << droppedName << std::endl;
							// Kopiowanie pliku do folderu docelowego
							std::filesystem::copy(fullPath, destination, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
							std::filesystem::remove_all(fullPath);
						}
						catch (const std::filesystem::filesystem_error& e) {
							std::cerr << "B³¹d przenoszenia pliku: " << e.what() << std::endl;
						}
					}
					else {
						std::cerr << "B³¹d: Œcie¿ka pliku jest pusta." << std::endl;
					}
					
				}
				ImGui::EndDragDropTarget(); // Upewnij siê, ¿e to jest pox
			}


			ImGui::PopID();
		}
	}
	ImGui::End(); // Upewnij siê, ¿e ten kod jest odpowiednio umiejscowiony
}



void editor::RemoveSceneObject(SceneObject* obj) {
	// Usuwanie dzieci rekurencyjnie

	obj->removeObject();
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
			"Texture",
		};

		// Tworzymy rozwijane menu do wyboru komponentu
		int currentIndex = static_cast<int>(selectedComponentType);
		if (ImGui::Combo("Select Component", &currentIndex, componentNames.data(), componentNames.size())) {
			selectedComponentType = static_cast<ComponentType>(currentIndex);

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