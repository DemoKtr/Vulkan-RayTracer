#pragma once

#include "SceneObject.h"

class Scene {
	ecs::ECS* ecs;
	bool b = false;
public:
	Scene();
	~Scene();
	SceneObject* root;
	void updateScene(float deltaTime);
};