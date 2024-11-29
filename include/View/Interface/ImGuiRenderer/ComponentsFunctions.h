#pragma once
#include <Scene/ECS/components/componentFabric.h>
#include "Scene/ECS/ecs.h"
namespace vkImGui {

	void AddComponent(ecs::ECS* ecs, ecs::Entity entity, ComponentType& selectedComponentType);
	void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
}