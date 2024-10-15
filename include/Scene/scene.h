#pragma once

#include "SceneObject.h"

class Scene {
	
	bool b = false;
public:
	ecs::ECS* ecs;
	Scene();
	~Scene();
	SceneObject* root;
	void updateScene(float deltaTime);
};