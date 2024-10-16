#include "app.h"
#include <sstream>
#include <settings.h>
#include "imgui.h"
#include <backends/imgui_impl_vulkan.h>



void App::build_glfw_window(glm::ivec2 screenSize, bool debugMode)
{
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void App::calculateFrameRate()
{
	currentTime = glfwGetTime();
	double delta = currentTime - lastTime;

	if (delta >= 1) {
		int framerate{ std::max(1, int(numFrames / delta)) };

		std::stringstream title{};
		title << "Running at " << framerate << " fps.  There are" << verticesCounter << "vertices send to gpu";
		glfwSetWindowTitle(window, title.str().c_str());

		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(1000.0 / framerate);
	}

	++numFrames;
}

void App::build_imgui_context() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Ustaw styl
	ImGui::StyleColorsDark();
	ImGui_ImplVulkan_InitInfo init_info = {};
}

App::App(glm::ivec2 screenSize, bool debugMode)
{
	vkSettings::lastX = screenSize.x / 2.0f;
	vkSettings::lastY = screenSize.y / 2.0f;
	build_glfw_window(screenSize, debugMode);

	scene = new Scene();
	
	graphicsEngine = new GraphicsEngine(screenSize, window, scene, debugMode);
	graphicsEngine->InitImGui(window);

}

App::~App()
{
	delete graphicsEngine;
	delete scene;
}

void App::run()
{
	size_t i = 0;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glfwPollEvents();
		scene->updateScene(deltaTime);
		graphicsEngine->render(scene, verticesCounter, deltaTime, camera,f12_button[1]);
		calculateFrameRate();
		

	}
}

void processInput(GLFWwindow* window)
{
	App* referenceApp = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		referenceApp->camera.ProcessKeyboard(Camera::Camera_Movement::FORWARD, referenceApp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		referenceApp->camera.ProcessKeyboard(Camera::Camera_Movement::BACKWARD, referenceApp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		referenceApp->camera.ProcessKeyboard(Camera::Camera_Movement::LEFT, referenceApp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		referenceApp->camera.ProcessKeyboard(Camera::Camera_Movement::RIGHT, referenceApp->deltaTime);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !referenceApp->f12_button[0]){
		referenceApp->f12_button[0] = true;
		referenceApp->f12_button[1] = !referenceApp->f12_button[1];
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		referenceApp->f12_button[0] = false;
	}


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

	App* referenceApp = static_cast<App*>(glfwGetWindowUserPointer(window));
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (vkSettings::firstMouse)
	{
		vkSettings::lastX = xpos;
		vkSettings::lastY = ypos;
		vkSettings::firstMouse = false;
	}

	float xoffset = xpos - vkSettings::lastX;
	float yoffset = vkSettings::lastY - ypos; // reversed since y-coordinates go from bottom to top

	vkSettings::lastX = xpos;
	vkSettings::lastY = ypos;

	referenceApp->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//App* referenceApp = static_cast<App*>(glfwGetWindowUserPointer(window));
	//referenceApp->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
