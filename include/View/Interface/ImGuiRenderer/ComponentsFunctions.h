#pragma once
#include <Scene/ECS/components/componentFabric.h>
#include "Scene/ECS/ecs.h"
#include <Scene/sceneObject.h>
#include "View/vkMesh/meshesManager.h"
namespace vkImGui {

	void AddComponent(ecs::ECS* ecs, SceneObject* selectedObject, vkMesh::MeshesManager* meshesManager, ComponentType& selectedComponentType);
	void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
}