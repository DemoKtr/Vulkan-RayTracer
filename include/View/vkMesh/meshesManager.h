#pragma once
#include "config.h"
#include <Scene/Transform.h>
#include "Scene/sceneobject.h"

namespace vkMesh {
	struct MeshManagerData {
		SceneObject* sceneObject;
		glm::mat4* modelMatrix;
	};

	class MeshesManager {

	public:
		std::unordered_map<int, std::vector<MeshManagerData>> modelMatrices;

		MeshesManager(SceneObject* root, ecs::ECS* ecs);

		void addMeshesRecursively(SceneObject* obj, ecs::ECS* ecs);
		void removeSceneObject(SceneObject* obj, ecs::ECS* ecs);
		void updateMeshIndex(SceneObject* obj, int newIndex, ecs::ECS* ecs);
	};
}
