#include "Scene/sceneObject.h"
#include "View/vkMesh/meshesManager.h"
#include <Scene/ECS/components/components.h>
#include <thread>
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
	std::shared_ptr<TextureComponent> texturecomponent = std::make_unique<TextureComponent>();
	ecs->addComponent(id, texturecomponent);
	ecs->addComponent(id, transformComponent);

	ecs->addComponent(id, meshcomponent);
	ecs->getComponent<MeshComponent>(id).get()->setIndex(1600640060211770124);

	meshesManager->addMesh(this,ecs);
	//ecs->getComponent<TransformComponent>(id).get()->getModifyableTransform().setLocalScale(glm::vec3(0.8f,.8f,.8f));
	//ecs->getComponent<TransformComponent>(id).get()->getModifyableTransform().setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//ecs->getComponent<TransformComponent>(id).get()->getModifyableTransform().computeModelMatrix();

	name = "SceneObject " + std::to_string(id);
}

SceneObject::SceneObject(ecs::ECS* ecs) {
	id = ecs->createEntity();
	std::shared_ptr<TransformComponent> transformComponent = std::make_unique<TransformComponent>();
	std::shared_ptr<MeshComponent> meshcomponent = std::make_unique<MeshComponent>();
	std::shared_ptr<TextureComponent> texturecomponent = std::make_unique<TextureComponent>();
	ecs->addComponent(id, texturecomponent);
	ecs->addComponent(id, transformComponent);
	ecs->addComponent(id, meshcomponent);
	ecs->getComponent<MeshComponent>(id).get()->setIndex(1600640060211770124);
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
	//if (!isActive) return; // Jeœli obiekt jest nieaktywny, pomiñ aktualizacjê.


	TransformComponent* component = ecs->getComponent<TransformComponent>(this->id).get();
	// Oblicz globaln¹ macierz modelu dla tego obiektu.
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