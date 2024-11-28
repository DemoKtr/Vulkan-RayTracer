#pragma once

#include "PrefabRenderer.h"

namespace vkPrefab {
	class PrefabWindow {
		GLFWwindow* window;
		Scene* scene;
		PrefabRenderer* renderer;
			void build_glfw_window(glm::ivec2 screenSize, bool debugMode);
	public:
		PrefabWindow(glm::ivec2 screenSize, bool debugMode);
		~PrefabWindow();
		void run();
	};
}
