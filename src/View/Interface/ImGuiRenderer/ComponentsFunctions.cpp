#include "View/Interface/ImGuiRenderer/ComponentsFunctions.h"
#include "Scene/ECS/ecs.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <settings.h>


void vkImGui::AddComponent(ecs::ECS* ecs, ecs::Entity entity, ComponentType& selectedComponentType) {
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
		};

		// Tworzymy rozwijane menu do wyboru komponentu
		int currentIndex = static_cast<int>(selectedComponentType);
		if (ImGui::Combo("Select Component", &currentIndex, componentNames.data(), componentNames.size())) {
			selectedComponentType = static_cast<ComponentType>(currentIndex);

		}

		// Przycisk do zamkniêcia selektora
		if (ImGui::Button("Add", ImVec2(100, 0))) {

			vkSettings::showComponentSelector = false; // Ukryj selektor po naciœniêciu
			ecs->addComponent(entity, createComponent(static_cast<ComponentType>(currentIndex)));


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
