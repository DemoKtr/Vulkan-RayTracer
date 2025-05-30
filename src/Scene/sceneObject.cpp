#include "Scene/sceneObject.h"
#include "View/vkMesh/meshesManager.h"
#include <Scene/ECS/components/components.h>
#include <thread>
#include "Scene/sceneObjectFlagBits.h"
SceneObject::SceneObject(ecs::ECS* ecs, SceneObject* parent) {
	id = ecs->createEntity();
	
	std::cout << ecs->getAllComponents(id).size() << std::endl;
	this->parent = parent;
	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager){

	id = ecs->createEntity();
	std::shared_ptr<TransformComponent> transformComponent = std::make_unique<TransformComponent>();
	std::shared_ptr<MeshComponent> meshcomponent = std::make_unique<MeshComponent>();

	//ecs->addComponent(id, dirtyFlagBits,transformComponent);

	//ecs->addComponent(id, dirtyFlagBits, meshcomponent);
	ecs->getComponent<MeshComponent>(id).get()->setIndex(1600640060211770124);

	meshesManager->addMesh(this,ecs);


	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs) {
	id = ecs->createEntity();
	std::shared_ptr<TransformComponent> transformComponent = std::make_unique<TransformComponent>();


	ecs->addComponent(id,transformComponent);

	name = "SceneObject " + std::to_string(id);
}

SceneObject::~SceneObject() {

}

void SceneObject::addChild(SceneObject* obj){
	obj->parent = this;
	children.push_back(obj);
}

void SceneObject::updateModelMatrix(ecs::ECS* ecs)
{
	//if (!isActive) return; // Je�li obiekt jest nieaktywny, pomi� aktualizacj�.


	TransformComponent* component = ecs->getComponent<TransformComponent>(this->id).get();
	// Oblicz globaln� macierz modelu dla tego obiektu.
	if (this->parent == nullptr)
		component->getModifyableTransform().computeModelMatrix();
	else {
		TransformComponent* parentComponent = ecs->getComponent<TransformComponent>(this->parent->id).get();
		component->getModifyableTransform().computeModelMatrix(parentComponent->getModifyableTransform().getModelMatrix());
	}


	// Rekurencyjnie zaktualizuj wszystkie dzieci.
	for (auto* child : children) {
		child->updateModelMatrix(ecs);
	}
}
std::string SceneObject::getName() {
	return name;
}

void SceneObject::removeObject() {
	
	while (!this->children.empty()) {

		SceneObject* child = this->children.back();
		this->children.pop_back();  // Usu� wska�nik z listy dzieci
		child->removeObject();  // Usu� dziecko rekurencyjnie
	}

	// Usu� obiekt z rodzica, je�li istnieje
	if (this->parent != nullptr) {
		auto& siblings = this->parent->children;

		// Znajd� i usu� wska�nik z listy dzieci rodzica
		auto it = std::find(siblings.begin(), siblings.end(), this);
		if (it != siblings.end()) {
			// Usu� wska�nik z wektora dzieci rodzica
			siblings.erase(it);
		}
	}

	// Zwolnij pami�� obiektu
	delete this;
	
}