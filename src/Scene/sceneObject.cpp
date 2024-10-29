#include "Scene/sceneObject.h"
#include "Scene/ECS/components/transformComponent.h"
#include <Scene/ECS/components/meshComponent.h>
#include <thread>
SceneObject::SceneObject(ecs::ECS* ecs, SceneObject* parent) {
	id = ecs->createEntity();
	
	std::cout << ecs->getAllComponents(id).size() << std::endl;
	this->parent = parent;
	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs){

	id = ecs->createEntity();
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
	std::shared_ptr<meshComponent> meshcomponent = std::make_shared<meshComponent>();

	ecs->addComponent(id, transformComponent);

	ecs->addComponent(id, meshcomponent);

	name = "SceneObject " + std::to_string(id);
}

SceneObject::~SceneObject() {

}

void SceneObject::addChild(SceneObject* obj){
	obj->parent = this;
	children.push_back(obj);
}

std::string SceneObject::getName() {
	return name;
}

void SceneObject::removeObject() {
	
	while (!this->children.empty()) {

		SceneObject* child = this->children.back();
		this->children.pop_back();  // Usuñ wskaŸnik z listy dzieci
		child->removeObject();  // Usuñ dziecko rekurencyjnie
	}

	// Usuñ obiekt z rodzica, jeœli istnieje
	if (this->parent != nullptr) {
		auto& siblings = this->parent->children;

		// ZnajdŸ i usuñ wskaŸnik z listy dzieci rodzica
		auto it = std::find(siblings.begin(), siblings.end(), this);
		if (it != siblings.end()) {
			// Usuñ wskaŸnik z wektora dzieci rodzica
			siblings.erase(it);
		}
	}

	// Zwolnij pamiêæ obiektu
	delete this;
	
}