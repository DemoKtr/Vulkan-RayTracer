#include "Scene/sceneObject.h"
#include "Scene/ECS/components/transformComponent.h"
SceneObject::SceneObject(ecs::ECS* ecs, SceneObject* parent) {
	id = ecs->createEntity();
	
	std::cout << ecs->getAllComponents(id).size() << std::endl;
	this->parent = parent;
	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs){
	id = ecs->createEntity();
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	ecs->addComponent(id, transformComponent);
	name = "SceneObject " + std::to_string(id);
}

void SceneObject::addChild(SceneObject* obj){
	obj->parent = this;
	children.push_back(obj);
}

std::string SceneObject::getName() {
	return name;
}
