#include "View/Interface/ImGuiRenderer/sceneObjectsFunctions.h"

void vkImGui::RemoveSceneObject(SceneObject* &obj, SceneObject* &selectedObject) {
	// Usuwanie dzieci rekurencyjnie
	obj->removeObject();
	if(selectedObject == obj)
	selectedObject = nullptr;
}

void vkImGui::AddSceneObject(SceneObject* obj, ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager) {
	SceneObject* newChild = new SceneObject(ecs, meshesManager);
	obj->addChild(newChild); // Dodaj nowy obiekt do dzieci
}
