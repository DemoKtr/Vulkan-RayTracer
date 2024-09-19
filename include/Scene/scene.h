#pragma once

#include "SceneObject.h"

class Scene
{


public:
	Scene();
	~Scene();
	std::vector<SceneObject*> sceneObjects;
	void updateScene(float deltaTime);
};