#pragma once
#include "config.h"
#include <Scene/Transform.h>
#include "Scene/sceneobject.h"
#include <map>

namespace vkMesh {
	struct MeshManagerData {
		SceneObject* sceneObject;
		glm::mat4* modelMatrix;
	};

	class MeshesManager {

	public:
		std::map<uint64_t, std::vector<MeshManagerData>> modelMatrices;

		MeshesManager(SceneObject* root, ecs::ECS* ecs);

		void addMeshesRecursively(SceneObject* obj, ecs::ECS* ecs);
		void removeSceneObject(SceneObject* obj, ecs::ECS* ecs);
		void updateMeshIndex(SceneObject* obj, uint64_t newIndex, ecs::ECS* ecs);
		void addMesh(SceneObject* obj, ecs::ECS* ecs);
	};
}
