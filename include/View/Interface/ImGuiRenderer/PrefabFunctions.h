#pragma once
#include "Scene/sceneObject.h"
#include "View/vkMesh/meshesManager.h"
#include "Scene/Objects/Prefab.h"
namespace vkImGui {
	void open_prefab_window(std::vector<vkPrefab::Prefab*>& prefabs, std::string prefabPath);
}