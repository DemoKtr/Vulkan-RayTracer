#pragma once
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include "config.h"
#include <glm/fwd.hpp>
#include "Scene/Transform.h"
#include "Scene/ECS/ecs.h"
#include <string>

namespace vkMesh {
	class MeshesManager;
}


class SceneObject{

	std::string name;
	
	

	public:
		void destroyChilds();
		bool isActive{ true };
		bool renderingDirtyFlag = true;
		ecs::Entity id;
		std::vector<SceneObject*> children;
		SceneObject* parent = nullptr;

		SceneObject(ecs::ECS* ecs, SceneObject* parent);
		SceneObject(ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager);
		SceneObject(ecs::ECS* ecs);
		SceneObject();
		~SceneObject();
		void draw();
		void addChild(SceneObject* obj);
		void update(float deltaTime);
		void updateModelMatrix(ecs::ECS* ecs);

		void removeObject();

		std::string getName();
};
#endif