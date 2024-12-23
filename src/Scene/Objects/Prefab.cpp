#pragma once
#include "Scene/Objects/Prefab.h"
#include "Scene/Objects/PrefabResources.h"

void vkPrefab::Prefab::build_glfw_window(glm::ivec2 screenSize, bool debugMode) {

	glfwInit();

	//no default rendering client, we'll hook vulkan up
	//to the window later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//resizing breaks the swapchain, we'll disable it for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	if (window = glfwCreateWindow(screenSize.x, screenSize.y, "VulkanGame", nullptr, nullptr)) {
		if (debugMode) {
			std::cout << "Successfully made a glfw window , width: " << screenSize.x << ", height: " << screenSize.y << '\n';
		}
	}
	else {
		if (debugMode) {
			std::cout << "GLFW window creation failed\n";
		}
	}
	glfwSetWindowUserPointer(window, this);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void vkPrefab::Prefab::load_from_file(nlohmann::json serialized) {

}

void vkPrefab::Prefab::windowCloseCallback(GLFWwindow* window) {

}

void vkPrefab::Prefab::closeWindow() {

}

Scene* vkPrefab::Prefab::getScene() {
	return this->scene;
}

vkPrefab::Prefab::Prefab(nlohmann::json serialized, bool debugMode) {
	build_glfw_window(vkPrefab::data.screenSize ,debugMode);

	scene = new Scene();

	renderer = new PrefabRenderer(this->scene ,window);

	renderer->InitImGui(window);
}

vkPrefab::Prefab::Prefab( bool debugMode) {
	build_glfw_window(vkPrefab::data.screenSize, debugMode);

	scene = new Scene();

	renderer = new PrefabRenderer( this->scene, window);

	//renderer->InitImGui(window);
}

vkPrefab::Prefab::~Prefab() {
	delete renderer;
}

void vkPrefab::Prefab::render(float deltaTime) {
	renderer->render(this->scene,this->camera, deltaTime);
}

bool vkPrefab::Prefab::isClosed() {
	return this->isClose;
}



