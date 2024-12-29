#pragma once
#include "Scene/sceneObject.h"
#include "View/vkMesh/meshesManager.h"
namespace vkImGui {
	void RemoveSceneObject(SceneObject* &obj, SceneObject*&selectedObject);
	void AddSceneObject(SceneObject* obj, ecs::ECS* ecs);
}