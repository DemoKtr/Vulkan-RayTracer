#include "Scene/scene.h"

#include <Scene/ECS/components/components.h>
#
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
	if (obj->renderingDirtyFlag) {
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
		
		obj->renderingDirtyFlag = false;

	}
}
