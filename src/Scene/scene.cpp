#include "Scene/scene.h"
#include <Scene/ECS/components/transformComponent.h>
#include <Scene/ECS/components/meshComponent.h>

Scene::Scene(){

	ecs = new ecs::ECS();
	root = new SceneObject(ecs);
	
	//SceneObject* o1 = new SceneObject(ecs);
	//SceneObject* o2 = new SceneObject(ecs);
	//SceneObject* o3 = new SceneObject(ecs);
	//SceneObject* o4 = new SceneObject(ecs);

	ecs->getComponent<TransformComponent>(root->id).get()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//ecs->getComponent<TransformComponent>(o1->id).get()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	ecs->getComponent<TransformComponent>(root->id).get()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	//ecs->getComponent<TransformComponent>(o1->id).get()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	ecs->getComponent<TransformComponent>(root->id).get()->getModifyableTransform().computeModelMatrix();
	//ecs->getComponent<TransformComponent>(o1->id).get()->getModifyableTransform().computeModelMatrix();
	//root->addChild(o2);
	//root->addChild(o3);

	//root->addChild(o4);


	
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
	/*
	if (!b) {
		SceneObject* o1 = new SceneObject(ecs);
		
		root->children[0]->addChild(o1);
		b = true;
	}*/
}
