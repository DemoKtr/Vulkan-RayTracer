#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <iostream>
#include "Scene/ECS/components/componentType.h"
#include "Scene/ECS/components/components.h"


// Definicja mapy fabryk jako zmiennej statycznej
std::unordered_map<ComponentType, std::function<std::shared_ptr<Component>()>> createComponentFactory();

std::shared_ptr<Component> createComponent(ComponentType type);