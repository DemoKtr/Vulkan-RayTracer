#include "Scene/scene.h"

Scene::Scene(){

	ecs = new ecs::ECS();
	root = new SceneObject(ecs);
	SceneObject* o1 = new SceneObject(ecs);
	SceneObject* o2 = new SceneObject(ecs);
	SceneObject* o3 = new SceneObject(ecs);
	SceneObject* o4 = new SceneObject(ecs);

	root->addChild(o1);
	root->addChild(o2);
	root->addChild(o3);
	root->addChild(o4);

	std::cout << root->getName()<<std::endl;
	std::cout << root->children[0]->getName() << std::endl;
	std::cout << root->children[1]->getName() << std::endl;
	std::cout << root->children[2]->getName() << std::endl;
	std::cout << root->children[3]->getName() << std::endl;
	
}

Scene::~Scene() {

	for (SceneObject* obj : root->children) {
		delete obj;
	}
	delete root;
	delete ecs;
}

void Scene::updateScene(float deltaTime) {
	if (!b) {
		SceneObject* o1 = new SceneObject(ecs);
		root->children[0]->addChild(o1);
		b = true;
	}
}
