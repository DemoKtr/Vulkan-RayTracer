#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Scene/ECS/component.h>

class Prefab {

	std::string name;
public:
	Prefab();
	bool isActive{ true };
	std::vector<Prefab> children;
	Prefab* parent = nullptr;
	std::vector<Component> components;
	
};
