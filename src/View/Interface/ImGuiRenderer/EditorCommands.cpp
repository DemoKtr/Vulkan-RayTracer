﻿#include "View/Interface/ImGuiRenderer/EditorCommands.h"

#include <iostream>
#include <MultithreatedSystems/Console.h>
#include <filesystem>
#include <Scene/ECS/components/components.h>
#include <Scene/ECS/components/componentFabric.h>
#include "View/Interface/ImGuiRenderer/ComponentsFunctions.h"
#include "Scene/Objects/PrefabManager.h"
#include "View/Interface/ImGuiRenderer/filesOperationsFunctions.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "View/Interface/ImGuiRenderer/sceneObjectsFunctions.h"
#include "fileOperations/Resources.h"
#include "Scene/sceneObjectFlagBits.h"

void vkImGui::render_editor(vkThumbs::ThumbsManager* miniatureManager,
	vkImGui::FilesExploresData& filesExploresData, 
	SceneObject* root, SceneObject* &selectedObj,
	ComponentType& selectedComponentType, ecs::ECS* ecs,
	RenderObjects* objects) {
	float screenHeight = ImGui::GetIO().DisplaySize.y;
	float screenWidth = ImGui::GetIO().DisplaySize.x;


	

	if (ImGui::BeginMainMenuBar()) {
		vkSettings::menuHeight = ImGui::GetWindowHeight();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("Create")) {
				if (ImGui::MenuItem("Prefab")) {
					fileType = fileOperations::FileType::Type::Prefab;
					fileSaverIsOpen = true;
				}
				if (ImGui::MenuItem("Script")) {
					fileType = fileOperations::FileType::Type::Script;
					fileSaverIsOpen = true;
				}
				ImGui::EndMenu();
			}

			// Submenu dla "Open"
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				// Akcja Open
			}

			if (ImGui::BeginMenu("Save Options")) {  // Tworzymy submenu
				if (ImGui::MenuItem("Save")) {
					// Akcja Save
				}
				if (ImGui::MenuItem("Save as..")) {
					// Akcja Save as..
				}
				ImGui::EndMenu();  // Kończymy submenu
			}

			ImGui::EndMenu();  // Kończymy menu główne
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Create")) {
				// Akcja Create
			}

			// Submenu dla "Open"
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				// Akcja Open
			}

			if (ImGui::BeginMenu("Save Options")) {  // Tworzymy submenu
				if (ImGui::MenuItem("Save")) {
					// Akcja Save
				}
				if (ImGui::MenuItem("Save as..")) {
					// Akcja Save as..
				}
				ImGui::EndMenu();  // Kończymy submenu
			}

			ImGui::EndMenu();  // Kończymy menu główne
		}
		ImGui::EndMainMenuBar();
	}

	//ImGui::PopStyleColor(); // Przywrócenie poprzedniego stylu
	if(fileSaverIsOpen)
		render_file_save_window(filesExploresData.currentFolder.string(), fileType, fileSaverIsOpen);


	// Pozycja i rozmiar dolnego panelu
	ImVec2 bottomPanelPos = ImVec2(0, screenHeight - vkSettings::bottomPanelHeight); // Na dole ekranu
	ImVec2 bottomPanelSize = ImVec2(screenWidth - vkSettings::rightPanelWidth, vkSettings::bottomPanelHeight); // Szerokość do prawego panelu
	
	// Renderowanie menu głównego (opcjonalne, jeśli chcesz mieć menu nad panelem)
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Project")) {
			if (ImGui::MenuItem("New Project")) {
				// Akcja dla "New Project"
			}
			if (ImGui::MenuItem("Open Project")) {
				// Akcja dla "Open Project"
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console")) {
			if (ImGui::MenuItem("Show Console")) {
				// Akcja dla "Show Console"
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Panel dolny
	ImGui::SetNextWindowPos(bottomPanelPos); // Pozycja dolnego panelu
	ImGui::SetNextWindowSize(bottomPanelSize); // Rozmiar dolnego panelu
	ImGui::Begin("Bottom Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	
	// Przyciski w dolnym panelu
	ImGui::BeginChild("Header", ImVec2(0, 18), false, ImGuiWindowFlags_NoScrollWithMouse);
	if (ImGui::Button("Project")) {
		vkSettings::renderConsole = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Console")) {
		vkSettings::renderConsole = true;
	}
	ImGui::EndChild();

	ImGui::BeginChild("Content", ImVec2(0, 0), true); // Zawartość przewijalna

	
	if (vkSettings::renderConsole) {
		
		render_console();

	}
	else {
		
		render_file_explorator(miniatureManager, filesExploresData);
		
	}
	
	ImGui::EndChild();
	
	// Zaktualizowanie zmiennej wysokości dolnego panelu
	ImVec2 bottomPanelActualSize = ImGui::GetWindowSize();
	vkSettings::bottomPanelHeight = bottomPanelActualSize.y; // Zaktualizowanie wysokości

	ImGui::End();


	// Panel lewy
	ImVec2 leftPanelPos = ImVec2(0, vkSettings::menuHeight); // Na lewo od ekranu
	ImVec2 leftPanelSize = ImVec2(vkSettings::leftPanelWidth, screenHeight - vkSettings::bottomPanelHeight - vkSettings::menuHeight); // Wysokość do dolnego panelu
	ImGui::SetNextWindowPos(leftPanelPos);
	ImGui::SetNextWindowSize(leftPanelSize);
	ImGui::Begin("Left Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	
	render_scenegraph(root, selectedObj,ecs);

	// Jeśli zmienia się rozmiar panelu lewego:
	ImVec2 leftPanelActualSize = ImGui::GetWindowSize();
	vkSettings::leftPanelWidth = leftPanelActualSize.x; // Zaktualizowanie zmiennej rozmiaru lewego panelu
	ImGui::End();

	// Panel prawy
	ImVec2 rightPanelPos = ImVec2(screenWidth - vkSettings::rightPanelWidth, vkSettings::menuHeight); // Na prawo od ekranu
	ImVec2 rightPanelSize = ImVec2(vkSettings::rightPanelWidth, screenHeight); // Wysokość do dolnego panelu
	ImGui::SetNextWindowPos(rightPanelPos);
	ImGui::SetNextWindowSize(rightPanelSize);
	ImGui::Begin("Right Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	display_scene_object(selectedObj,ecs,objects,miniatureManager,selectedComponentType);
	// Jeśli zmienia się rozmiar panelu prawego:
	ImVec2 rightPanelActualSize = ImGui::GetWindowSize();
	vkSettings::rightPanelWidth = rightPanelActualSize.x; // Zaktualizowanie zmiennej rozmiaru prawego panelu
	
	ImGui::End();
	
	

	
}

void vkImGui::render_console() {
	// Blokada dostępu do wiadomości
	std::lock_guard<std::mutex> lock(console::consoleMutex);

	// Iteracja po wiadomościach i wyświetlanie ich w odpowiednich kolorach
	for (const auto& msg : console::consoleMessages) {
		ImGui::PushStyleColor(ImGuiCol_Text, msg.second);
		ImGui::TextWrapped("%s", msg.first.c_str());
		ImGui::PopStyleColor();
	}
}

void vkImGui::render_file_explorator(vkThumbs::ThumbsManager* miniatureManager, vkImGui::FilesExploresData& filesExploresData) {


		if (filesExploresData.currentFolder != filesExploresData.baseFolder) {
			if (ImGui::Button("..")) {
				auto parentFolder = filesExploresData.currentFolder.parent_path();
				if (parentFolder.string().find(filesExploresData.baseFolder.string()) == 0) {
					filesExploresData.currentFolder = parentFolder;
				}
			}
		}

		for (const auto& entry : std::filesystem::directory_iterator(filesExploresData.currentFolder)) {

			const auto& path = entry.path();
			std::string name = path.filename().string();




			ImVec2 imageSize(64, 64);

			ImTextureID imguiTextureId;
			ImGui::PushID(name.c_str());
			
			if (ImGui::Selectable((name + "/").c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
				if (entry.is_directory()) {
					imguiTextureId = miniatureManager->get_file_icon();
					ImGui::Image(imguiTextureId, imageSize);
					filesExploresData.currentFolder = path;
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



			rmb_click_render(path, filesExploresData);




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
							std::filesystem::path destination = filesExploresData.currentFolder / name / droppedName;



							std::filesystem::create_directories(destination);
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
				
				
			}

			
			ImGui::PopID();
		}
}

void vkImGui::render_scenegraph(SceneObject* root, SceneObject*& selectedObject, ecs::ECS* ecs) {
	//ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Przykładowo, większa czcionka
	if (ImGui::TreeNode(root->getName().c_str())) {
		if (ImGui::IsItemClicked()) {
			selectedObject = root;	
		}
		
		rmb_click_render(root, ecs);
		// Rekurencyjne rysowanie dzieci obiektu
		for (auto& child : root->children) {
			render_scenegraph(child, selectedObject,ecs);
		}


		ImGui::TreePop();

	}
	
	
}

void vkImGui::display_scene_object(SceneObject* &selectedObject, ecs::ECS* ecs, RenderObjects* objects, vkThumbs::ThumbsManager* miniatureManager, ComponentType& selectedComponentType) {
	if (selectedObject != nullptr) {
		fileOperations::FilesManager& filesManager = fileOperations::FilesManager::getInstance();
		ImGui::Text("Object Name: %s", selectedObject->getName().c_str());
		ImGui::Separator();
		ImGui::Text("Components:");
		auto components = ecs->getAllComponents(selectedObject->id);
		if (ImGui::Checkbox("Active: ", &selectedObject->isActive)){
			//selectedObject->renderingDirtyFlag = true;
			scene::setAllComponentsUpdated(selectedObject->dirtyFlagBits);
			
		}
		for (const auto& componentPtr : components) {
			Component* comp = componentPtr.get();  // Surowy wskaźnik

			std::string label = comp->getLabel();



			if (ImGui::TreeNode(label.c_str())) {
				// Wyświetlanie specyficznych pól dla komponentów
				if (comp->getType() == ComponentType::Transform) {
					TransformComponent* transform = dynamic_cast<TransformComponent*>(comp);
					if (transform != nullptr) {
						// Wyświetlanie i edytowanie transformacji
						Transform& transformData = transform->getModifyableTransform();
						glm::vec3 eulerAngles = glm::eulerAngles(transformData.getModifyableLocalRotation());
						ImGui::DragFloat3("Position", &transformData.getModifyableLocalPosition().x, 0.1f);
						if (ImGui::DragFloat3("Rotation", &eulerAngles.x, 0.1f)) {
							glm::quat newRotation = glm::yawPitchRoll(eulerAngles.y, eulerAngles.x, eulerAngles.z);
							transformData.setLocalRotation(newRotation);
							// Przekształcenie kątów Eulerów do kwaternionu tylko po zmianie wartości
							transformData.setLocalRotation(glm::quat(eulerAngles));
						}

						ImGui::DragFloat3("Scale", &transformData.getModifyableLocalScale().x, 0.1f);

					
						selectedObject->updateModelMatrix(ecs);
					}
					ImGui::SameLine;
					//this->RemoveComponent(scene->ecs, selectedObject->id, transform);
				}
				else if (comp->getType() == ComponentType::Mesh) {
					static char searchQuery[128] = "";
					MeshComponent* mesh = dynamic_cast<MeshComponent*>(comp);

					// Wyświetl aktualnie przypisany model jako rozwijaną listę
					std::string currentModel;
					if (mesh->getIndex() >= 0)
						currentModel = (filesManager.getMeshesNames().fileNames[filesManager.getMeshesNames().getIndex(mesh->getIndex())]);
					else currentModel = "";

					if (ImGui::BeginCombo("##modelSelector", currentModel.c_str())) { // Combo bez etykiety z lewej strony
						// Pole wyszukiwania w rozwijanej liście
						ImGui::InputText("Search Models", searchQuery, IM_ARRAYSIZE(searchQuery));

						// Pętla do wyświetlania wyników wyszukiwania
						for (size_t i = 0; i < filesManager.getMeshesNames().fileNames.size(); ++i) {
							const std::string& modelName = filesManager.getMeshesNames().fileNames[i];
							if (modelName.find(searchQuery) != std::string::npos) {

								ImGui::PushID(i);

								// Połączony tekst nazwy modelu i ścieżki
								std::string displayText = modelName + "\nPath: " + filesManager.getMeshesNames().fullPaths[i];
								ImVec2 imageSize(64, 64);
								// Wybór modelu

								ImGui::Image(miniatureManager->get_mesh_icon(i), imageSize);

								ImGui::SameLine;
								if (ImGui::Selectable(displayText.c_str(), mesh->getIndex() == i)) {
									//meshesManager->updateMeshIndex(selectedObject, fileOperations::meshesNames.hash[fileOperations::meshesNames.fullPaths[i]], ecs);
									objects->changeModelId(ecs, filesManager.getMeshesNames().hash[filesManager.getMeshesNames().fullPaths[i]],selectedObject);

									std::cout << "Wybrano indeks: " << mesh->getIndex() << std::endl;

								}

								ImGui::PopID();
							}
						}
						ImGui::EndCombo();
					}

					// Przycisk do usuwania komponentu obok selektora


					//RemoveComponent(ecs, selectedObject->id, mesh);

				}
				else if (comp->getType() == ComponentType::Particle) {
					ParticleComponent* particleComp = dynamic_cast<ParticleComponent*>(comp);
					if (particleComp != nullptr) {
						// Uzyskaj dostęp do emitera
						vkParticle::ParticleEmmiter* emiter = particleComp->getEmiter();
						if (emiter != nullptr) {
							// Edycja liczby cząsteczek w emiterze
							ImGui::Text("Particle Emitter Settings");
						

							// Edycja pozycji emitera
							glm::vec4 emitterPosition = emiter->postion;
							if (ImGui::DragFloat3("Emitter Position", &emitterPosition.x, 0.1f)) {
								emiter->postion = emitterPosition;
							}

							// Wyświetlanie generatorów
							ImGui::Separator();
							ImGui::Text("Particle Generators:");
							auto& generators = emiter->getGenerators();
							for (size_t i = 0; i < generators.size(); ++i) {
								ImGui::PushID(static_cast<int>(i)); // Unikalne ID dla generatora
								if (ImGui::TreeNode(("Generator " + std::to_string(i)).c_str())) {
									auto& generator = generators[i];

									// Zakresy pozycji
									glm::vec4 positionMin, positionMax;
									generator.GetPositionRange(positionMin, positionMax);
									if (ImGui::DragFloat3("Position Min", &positionMin.x, 0.1f) ||
										ImGui::DragFloat3("Position Max", &positionMax.x, 0.1f)) {
										generator.UpdateValue(generator.positionMin, positionMin, generator.dirty_flag);
										generator.UpdateValue(generator.positionMax, positionMax, generator.dirty_flag);
									}

									// Zakresy prędkości
									glm::vec4 velocityMin, velocityMax;
									generator.GetVelocityRange(velocityMin, velocityMax);
									if (ImGui::DragFloat3("Velocity Min", &velocityMin.x, 0.1f) ||
										ImGui::DragFloat3("Velocity Max", &velocityMax.x, 0.1f)) {
										generator.UpdateValue(generator.velocityMin, velocityMin, generator.dirty_flag);
										generator.UpdateValue(generator.velocityMax, velocityMax, generator.dirty_flag);
									}

									// Zakresy kolorów
									glm::vec4 colorMin, colorMax;
									generator.GetColorRange(colorMin, colorMax);
									if (ImGui::ColorEdit4("Color Min", &colorMin.x) ||
										ImGui::ColorEdit4("Color Max", &colorMax.x)) {
										generator.UpdateValue(generator.colorMin, colorMin, generator.dirty_flag);
										generator.UpdateValue(generator.colorMax, colorMax, generator.dirty_flag);
									}

									// Inne właściwości (rozmiar, rotacja, czas życia itd.)
									float lifetimeMin, lifetimeMax;
									generator.GetLifetimeRange(lifetimeMin, lifetimeMax);
									if (ImGui::DragFloat("Lifetime Min", &lifetimeMin, 0.1f) ||
										ImGui::DragFloat("Lifetime Max", &lifetimeMax, 0.1f)) {
										generator.UpdateValue(generator.lifetimeMin, lifetimeMin, generator.dirty_flag);
										generator.UpdateValue(generator.lifetimeMax, lifetimeMax, generator.dirty_flag);
									}

									ImGui::TreePop();
								}
								ImGui::PopID();
							}

							// Dodawanie/usuwanie generatorów
							if (ImGui::Button("Add Generator")) {
								vkParticle::ParticleGenerator newGenerator(emiter->dirtyflag);
								emiter->AddGenerator(newGenerator);
							}
							if (generators.size() > 0) {
								if (ImGui::Button("Remove Last Generator")) {
									emiter->RemoveGenerator(generators.size() - 1);
								}
							}
						}
					}
				}
				/**/
				else if (comp->getType() == ComponentType::Texture) {
					static char searchQuery[128] = "";
					TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(comp);

					if (ImGui::Checkbox("Is PBR Texture", textureComponent->isPBRTexture())) {
						//selectedObject->renderingDirtyFlag = true;
						scene::updateComponent(scene::SceneObjectFlagBits::rendering,selectedObject->dirtyFlagBits);
					}
					

					// Wyświetl aktualnie przypisany model jako rozwijaną listę
					std::string currentTexture;
					if (textureComponent->getColorTextureIndex() >= 0)
						currentTexture = filesManager.getTexturesNames().fileNames[filesManager.getTexturesNames().getIndex(textureComponent->getColorTextureIndex())];
					else currentTexture = "";

					if (ImGui::BeginCombo("##textureSelector", currentTexture.c_str())) { // Combo bez etykiety z lewej strony
						// Pole wyszukiwania w rozwijanej liście
						ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

						// Pętla do wyświetlania wyników wyszukiwania
						for (size_t i = 0; i < filesManager.getTexturesNames().fileNames.size(); ++i) {
							const std::string& textureName = filesManager.getTexturesNames().fileNames[i];
							if (textureName.find(searchQuery) != std::string::npos) {

								ImGui::PushID(i);

								// Połączony tekst nazwy modelu i ścieżki
								std::string displayText = textureName + "\nPath: " + filesManager.getTexturesNames().fullPaths[i];


								ImVec2 imageSize(64, 64);

								// Wybór modelu
								ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
								ImGui::SameLine;
								if (ImGui::Selectable(displayText.c_str(), filesManager.getTexturesNames().getIndex(textureComponent->getColorTextureIndex()) == i)) {
									textureComponent->setColorTextureIndex(filesManager.getTexturesNames().hash[filesManager.getTexturesNames().fullPaths[i]]);  // Zapisz wybrany indeks
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
							currentNormalTexture = filesManager.getTexturesNames().fileNames[filesManager.getTexturesNames().getIndex(textureComponent->getNormalTextureIndex())];
						else currentNormalTexture = "";

						if (ImGui::BeginCombo("##normalSelector", currentNormalTexture.c_str())) { // Combo bez etykiety z lewej strony
							// Pole wyszukiwania w rozwijanej liście
							ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

							// Pętla do wyświetlania wyników wyszukiwania
							for (size_t i = 0; i < filesManager.getTexturesNames().fileNames.size(); ++i) {
								const std::string& textureName = filesManager.getTexturesNames().fileNames[i];
								if (textureName.find(searchQuery) != std::string::npos) {

									ImGui::PushID(i);

									// Połączony tekst nazwy modelu i ścieżki
									std::string displayText = textureName + "\nPath: " + filesManager.getTexturesNames().fullPaths[i];

									ImVec2 imageSize(64, 64);

									// Wybór modelu
									ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
									ImGui::SameLine;
									if (ImGui::Selectable(displayText.c_str(), filesManager.getTexturesNames().getIndex(textureComponent->getNormalTextureIndex()) == i)) {
										textureComponent->setNormalTextureIndex(filesManager.getTexturesNames().hash[filesManager.getTexturesNames().fullPaths[i]]);  // Zapisz wybrany indeks
										std::cout << "Wybrano indeks: " << textureComponent->getNormalTextureIndex() << std::endl;
									}

									ImGui::PopID();
								}
							}
							ImGui::EndCombo();
						}

						std::string currentARMTexture;
						if (textureComponent->getARMTextureIndex() >= 0)
							currentARMTexture = filesManager.getTexturesNames().fileNames[filesManager.getTexturesNames().getIndex(textureComponent->getARMTextureIndex())];
						else currentARMTexture = "";

						if (ImGui::BeginCombo("##ARMtextureSelector", currentARMTexture.c_str())) { // Combo bez etykiety z lewej strony
							// Pole wyszukiwania w rozwijanej liście
							ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

							// Pętla do wyświetlania wyników wyszukiwania
							for (size_t i = 0; i < filesManager.getTexturesNames().fileNames.size(); ++i) {
								const std::string& textureName = filesManager.getTexturesNames().fileNames[i];
								if (textureName.find(searchQuery) != std::string::npos) {

									ImGui::PushID(i);

									// Połączony tekst nazwy modelu i ścieżki
									std::string displayText = textureName + "\nPath: " + filesManager.getTexturesNames().fullPaths[i];

									
									ImVec2 imageSize(64, 64);
									// Wybór modelu
									ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
									ImGui::SameLine;
									if (ImGui::Selectable(displayText.c_str(), filesManager.getTexturesNames().getIndex(textureComponent->getARMTextureIndex()) == i)) {
										textureComponent->setARMTextureIndex(filesManager.getTexturesNames().hash[filesManager.getTexturesNames().fullPaths[i]]);  // Zapisz wybrany indeks
										std::cout << "Wybrano indeks: " << textureComponent->getARMTextureIndex() << std::endl;
									}

									ImGui::PopID();
								}
							}
							ImGui::EndCombo();
						}

						std::string currentDepthTexture;
						if (textureComponent->getDepthTextureIndex() >= 0)
							currentDepthTexture = filesManager.getTexturesNames().fileNames[filesManager.getTexturesNames().getIndex(textureComponent->getDepthTextureIndex())];
						else currentDepthTexture = "";

						if (ImGui::BeginCombo("##DepthtextureSelector", currentDepthTexture.c_str())) { // Combo bez etykiety z lewej strony
							// Pole wyszukiwania w rozwijanej liście
							ImGui::InputText("Search Textures", searchQuery, IM_ARRAYSIZE(searchQuery));

							// Pętla do wyświetlania wyników wyszukiwania
							for (size_t i = 0; i < filesManager.getTexturesNames().fileNames.size(); ++i) {
								const std::string& textureName = filesManager.getTexturesNames().fileNames[i];
								if (textureName.find(searchQuery) != std::string::npos) {

									ImGui::PushID(i);

									// Połączony tekst nazwy modelu i ścieżki
									std::string displayText = textureName + "\nPath: " + filesManager.getTexturesNames().fullPaths[i];

									ImVec2 imageSize(64, 64);
									// Wybór modelu
									ImGui::Image(miniatureManager->get_texture_icon(i), imageSize);
									ImGui::SameLine;
									if (ImGui::Selectable(displayText.c_str(), filesManager.getTexturesNames().getIndex(textureComponent->getDepthTextureIndex()) == i)) {
										textureComponent->setDepthTextureIndex(filesManager.getTexturesNames().hash[filesManager.getTexturesNames().fullPaths[i]]);  // Zapisz wybrany indeks
										std::cout << "Wybrano indeks: " << textureComponent->getDepthTextureIndex() << std::endl;
									}

									ImGui::PopID();
								}
							}
							ImGui::EndCombo();
						}


					}

				}
				ImGui::TreePop();
			}

		}
		AddComponent(ecs, selectedObject, selectedComponentType);

	}

}

void vkImGui::rmb_click_render(std::filesystem::path path, vkImGui::FilesExploresData& filesExploresData) {
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("ContextMenu");

	}
	if (ImGui::BeginPopup("ContextMenu")) {
		if (ImGui::MenuItem("Copy")) {
			filesExploresData.clipboard = path.string();
			filesExploresData.isCut = false; // Ustaw flagę na kopiowanie
		}
		if (ImGui::MenuItem("Cut")) {
			filesExploresData.clipboard = path.string();
			filesExploresData.isCut = true; // Ustaw flagę na wycinanie
		}
		if (ImGui::MenuItem("Delete")) {
			std::filesystem::remove(path);
		}
		if (ImGui::MenuItem("Paste")) {
			if (!filesExploresData.clipboard.empty()) {
				std::filesystem::path destination = filesExploresData.currentFolder / std::filesystem::path(filesExploresData.clipboard).filename();

				try {
					if (filesExploresData.isCut) {
						std::filesystem::rename(filesExploresData.clipboard, destination); // Przenieś plik
					}
					else {
						std::filesystem::copy(filesExploresData.clipboard, destination, std::filesystem::copy_options::overwrite_existing); // Skopiuj plik
					}
				}
				catch (const std::filesystem::filesystem_error& e) {
					std::cerr << "Błąd operacji: " << e.what() << std::endl;
				}

				if (filesExploresData.isCut) {
					filesExploresData.clipboard.clear(); // Wyczyść schowek po wycięciu
				}
			}
		}
		ImGui::EndPopup();
	}
}

void vkImGui::rmb_click_render(SceneObject* root, ecs::ECS* ecs) {

	SceneObject* obj = root;
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("ContextMenu");
		
	}

	if (ImGui::BeginPopup("ContextMenu")) {
		if (ImGui::BeginMenu("Create")) { // Menu "Create"
			if (ImGui::MenuItem("Object")) {

				vkImGui::AddSceneObject(obj, ecs);

			}

			if (ImGui::BeginMenu("2D Objects")) { // Submenu "2D Objects"
				if (ImGui::MenuItem("9-Silced")) {
					// Akcja dla "9-Silced"
				}
				if (ImGui::MenuItem("Capsule")) {
					// Akcja dla "Capsule"
				}
				if (ImGui::MenuItem("Circle")) {
					// Akcja dla "Circle"
				}
				if (ImGui::MenuItem("Square")) {
					// Akcja dla "Square"
				}
				if (ImGui::MenuItem("Triangle")) {
					// Akcja dla "Triangle"
				}
				ImGui::EndMenu(); // Zamknięcie submenu "2D Objects"
			}
			if (ImGui::BeginMenu("3D Objects")) {  // Tworzymy submenu
				if (ImGui::MenuItem("Cube")) {
					// Akcja Save
				}
				if (ImGui::MenuItem("Sphere")) {
					// Akcja Save as..
				}
				if (ImGui::MenuItem("Capsule")) {
					// Akcja Save as..
				}
				if (ImGui::MenuItem("Cylinder")) {
					// Akcja Save as..
				}
				if (ImGui::MenuItem("Plane")) {
					// Akcja Save as..
				}
				if (ImGui::MenuItem("Quad")) {
					// Akcja Save as..
				}
				ImGui::EndMenu();  // Kończymy submenu
			}

			ImGui::EndMenu(); // Zamknięcie menu "Create"

		}
		if (ImGui::MenuItem("Cut")) {

		}
		if (ImGui::MenuItem("Delete")) {

		}
		if (ImGui::MenuItem("Paste")) {

		}
		ImGui::EndPopup(); // Zamknięcie popupu "ContextMenu"
	}
}






