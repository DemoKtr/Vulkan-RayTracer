#include "View/Interface/ImGuiRenderer/sceneObjectsFunctions.h"
#include "Scene/ECS/components/transformComponent.h"
void vkImGui::RemoveSceneObject(SceneObject* &obj, SceneObject* &selectedObject) {
	// Usuwanie dzieci rekurencyjnie
	obj->removeObject();
	if(selectedObject == obj)
	selectedObject = nullptr;
}

void vkImGui::AddSceneObject(SceneObject* obj, ecs::ECS* ecs) {
	SceneObject* newChild = new SceneObject(ecs);
	
	obj->addChild(newChild); // Dodaj nowy obiekt do dzieci
	newChild->updateModelMatrix(ecs);
}
