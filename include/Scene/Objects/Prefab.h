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

		bool isClose = false;

		void build_glfw_window(glm::ivec2 screenSize, bool debugMode);

		void load_from_file(nlohmann::json serialized);

		static void windowCloseCallback(GLFWwindow* window);

		void closeWindow();
	public:
		Prefab(nlohmann::json serialized, bool debugMode);
		Prefab(bool debugMode);
		~Prefab();
		void render(float deltaTime);
		bool isClosed();





	};
}




#endif // !PREFAB_h