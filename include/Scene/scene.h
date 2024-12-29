#pragma once

#include "SceneObject.h"
#include "View/RenderingFlags.h"

class Scene {
	
	
public:
	ecs::ECS* ecs;
	Scene();
	~Scene();
	SceneObject* root;
	int getSceneObjectNumber(SceneObject* obj);
	void updateScene(float deltaTime);
	void update_objects_to_rendering(RenderObjects& objects_to_rendering, SceneObject* obj);
};