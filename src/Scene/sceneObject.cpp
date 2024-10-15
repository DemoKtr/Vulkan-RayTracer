#include "Scene/sceneObject.h"

SceneObject::SceneObject(ecs::ECS* ecs, SceneObject* parent) {
	id = ecs->createEntity();
	this->parent = parent;
	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs){
	id = ecs->createEntity();
	name = "SceneObject " + std::to_string(id);
}

void SceneObject::addChild(SceneObject* obj){
	obj->parent = this;
	children.push_back(obj);
}

Transform& SceneObject::getTransform() {
	return this->transform;
}

std::string SceneObject::getName() {
	return name;
}
