#include "View/Interface/ImGuiRenderer/ComponentsFunctions.h"
#include "Scene/ECS/ecs.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <settings.h>
#include "Scene/sceneObjectFlagBits.h"

void vkImGui::AddComponent(ecs::ECS* ecs, SceneObject* selectedObject, ComponentType& selectedComponentType) {
	// Wyœwietlenie przycisku do dodania komponentu
	if (ImGui::Button("Add Component")) {
		// Prze³¹cz widocznoœæ selektora komponentów
		vkSettings::showComponentSelector = !vkSettings::showComponentSelector; // Zmiana stanu
	}

	// Jeœli selektor komponentów ma byæ widoczny
	if (vkSettings::showComponentSelector) {
		// Lista typów komponentów do wyboru (pomijamy None)
		std::vector<const char*> componentNames = {
			"Transform",
			"Mesh",
			"Texture",
			"Render",
			"Physics",
			"Script",
			"Particle",
		};

		// Tworzymy rozwijane menu do wyboru komponentu
		int currentIndex = static_cast<int>(selectedComponentType);
		if (ImGui::Combo("Select Component", &currentIndex, componentNames.data(), componentNames.size())) {
			selectedComponentType = static_cast<ComponentType>(currentIndex);

		}

		// Przycisk do zamkniêcia selektora
		if (ImGui::Button("Add", ImVec2(100, 0))) {

			
				switch (selectedComponentType) {
				case ComponentType::Transform: {
					std::shared_ptr<TransformComponent> createdComponent = std::make_unique<TransformComponent>();
					ecs->addComponent(selectedObject->id, createdComponent);
					scene::updateComponent(scene::SceneObjectFlagBits::rendering, selectedObject->dirtyFlagBits);
					//selectedObject->renderingDirtyFlag = true;
					break;
				}
					
				case ComponentType::Mesh: {
					std::shared_ptr<MeshComponent> createdComponent = std::make_unique<MeshComponent>();
					ecs->addComponent(selectedObject->id,createdComponent);
					//meshesManager->addMesh(selectedObject, ecs);
					scene::updateComponent(scene::SceneObjectFlagBits::rendering, selectedObject->dirtyFlagBits);
					break;
				}
					
				case ComponentType::Texture: {
					std::shared_ptr<TextureComponent> createdComponent = std::make_unique<TextureComponent>();
					ecs->addComponent(selectedObject->id, createdComponent);
					scene::updateComponent(scene::SceneObjectFlagBits::rendering, selectedObject->dirtyFlagBits);
					break;
				}
				case ComponentType::Particle: {
					std::shared_ptr<ParticleComponent> createdComponent = std::make_unique<ParticleComponent>();
					ecs->addComponent(selectedObject->id, createdComponent);
					scene::updateComponent(scene::SceneObjectFlagBits::particle, selectedObject->dirtyFlagBits);
					break;
				}
				default:
					std::cerr << "Undefined ComponentType" << std::endl;
					;
				}
	
			


		}
		ImGui::SameLine();
		if (ImGui::Button("Close", ImVec2(100, 0))) {
			vkSettings::showComponentSelector = false; // Ukryj selektor po naciœniêciu
		}
	}
}

void vkImGui::RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component) {
		ecs->removeComponent(component, entity);
}
