#include "Scene/scene.h"

#include <Scene/ECS/components/components.h>
#include "Scene/sceneObjectFlagBits.h"
Scene::Scene() {

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

int Scene::getSceneObjectNumber(SceneObject* obj) {
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

void Scene::update_objects_to_rendering(RenderObjects& objects_to_rendering, SceneObject* obj) {
	for (SceneObject* child : obj->children) {
		update_objects_to_rendering(objects_to_rendering, child);
	}
	if (scene::isComponentUpdated(scene::SceneObjectFlagBits::rendering,obj->dirtyFlagBits)) {
		if (this->ecs->hasComponent<MeshComponent>(obj->id)) {
			if (this->ecs->hasComponent<TextureComponent>(obj->id) && obj->isActive) {
				if (this->ecs->getComponent<TextureComponent>(obj->id).get()->getPBR()) {
					objects_to_rendering.addObjectToGroup(this->ecs->getComponent<MeshComponent>(obj->id).get()->getIndex(), obj, GroupType::PBR);

				}
				else {
					objects_to_rendering.addObjectToGroup(this->ecs->getComponent<MeshComponent>(obj->id).get()->getIndex(), obj, GroupType::UNLIT);
				}
			}
			else {
				objects_to_rendering.addObjectToGroup(this->ecs->getComponent<MeshComponent>(obj->id).get()->getIndex(), obj, GroupType::INACTIVE);
			}
			
		}
		else {
			objects_to_rendering.addObjectToGroup(0, obj, GroupType::INACTIVE);
		}
		scene::resetComponent(scene::SceneObjectFlagBits::rendering, obj->dirtyFlagBits);
		//obj->renderingDirtyFlag = false;

	}
}

void Scene::update_particles_to_rendering(vkParticle::ParticleManager* manager, SceneObject* obj) {
	for (SceneObject* child : obj->children) {
		update_particles_to_rendering(manager, child);
	}
	if (scene::isComponentUpdated(scene::SceneObjectFlagBits::particle, obj->dirtyFlagBits) && obj->isActive) {
		if (this->ecs->hasComponent<ParticleComponent>(obj->id)) {	

			manager->AddParticleComponent(ecs,obj);
		}
		else {
			std::cout << "chujowe test" << std::endl;
		}
		scene::resetComponent(scene::SceneObjectFlagBits::particle, obj->dirtyFlagBits);
		//obj->renderingDirtyFlag = false;

	}
}
