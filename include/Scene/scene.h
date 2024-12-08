#pragma once

#include "SceneObject.h"

class Scene {
	
	
public:
	ecs::ECS* ecs;
	Scene();
	~Scene();
	SceneObject* root;
	int getSceneObjectNumber(SceneObject* obj);
	void updateScene(float deltaTime);
};