#include "Scene/scene.h"
#include <Scene/ECS/components/transformComponent.h>
#include <Scene/ECS/components/meshComponent.h>

Scene::Scene(){

	ecs = new ecs::ECS();
	root = new SceneObject(ecs);
	
	
}

Scene::~Scene() {

	for (SceneObject* obj : root->children) {
		delete obj;
	}
	delete root;
	delete ecs;
}

int Scene::getSceneObjectNumber(SceneObject* obj){
	if (!obj) return 0;

	int count = 1; 
	for (SceneObject* child : obj->children) {
		count += getSceneObjectNumber(child); 
	}
	return count;
}

void Scene::updateScene(float deltaTime) {
	
		//root->updateModelMatrix(ecs);
		
	
	
}
