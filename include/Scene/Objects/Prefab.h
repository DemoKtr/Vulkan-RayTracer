#pragma once
#ifndef PREFAB_h
#define PREFAB_h
#include "config.h"
#include "Player/camera.h"
#include <Scene/scene.h>
#include <View/Interface/PrefabWindow/PrefabRenderer.h>
#include <nlohmann/json.hpp>



namespace vkPrefab {
	

	class Prefab
	{

		GLFWwindow* window;
		Scene* scene;
		Camera::Camera camera;
		PrefabRenderer* renderer;

		void build_glfw_window(glm::ivec2 screenSize, bool debugMode);

		void deserialize(nlohmann::json serialized);

		static void windowCloseCallback(GLFWwindow* window);

		void closeWindow();
	public:
		Prefab(PrefabInput input,glm::ivec2 screenSize, bool debugMode);
		Prefab(PrefabInput input, nlohmann::json serialized,glm::ivec2 screenSize, bool debugMode);
		~Prefab();
		void render();
		





	};
}




#endif // !PREFAB_h