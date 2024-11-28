#include "View/Interface/PrefabWindow/PrefabWindow.h"

void vkPrefab::PrefabWindow::build_glfw_window(glm::ivec2 screenSize, bool debugMode) {
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
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

vkPrefab::PrefabWindow::PrefabWindow(glm::ivec2 screenSize, bool debugMode) {

	build_glfw_window(screenSize, debugMode);

	scene = new Scene();

	PrefabRendererInput input;

	renderer = new PrefabRenderer(input);
	renderer->InitImGui(window);
}

vkPrefab::PrefabWindow::~PrefabWindow() {

}

void vkPrefab::PrefabWindow::run() {

}
