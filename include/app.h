#pragma once
#ifndef APP_H
#define APP_H

#include "config.h"
#include "Player/camera.h"
#include <Scene/scene.h>
#include "View/egnine.h"

class App
{
	GraphicsEngine* graphicsEngine;
	GLFWwindow* window;
	Scene* scene;
	float lastFrame = 0.0f;
	double lastTime, currentTime;
	int numFrames;
	float frameTime;
	void build_glfw_window(glm::ivec2 screenSize, bool debugMode);
	void calculateFrameRate();
	int verticesCounter = {};
	void build_imgui_context();

	

public:
	App(glm::ivec2 screenSize, bool debugMode);
	~App();
	void run();
	Camera::Camera camera;
	float deltaTime = 0.0f;
	bool f12_button[2] = { {false }, { true } };
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);



#endif // !APP_H