#pragma once
#ifndef APP_H
#define APP_H

#include "config.h"
#include "Player/camera.h"
#include <Scene/scene.h>
#include "View/egnine.h"

#include "Input/inputManager.h"

class App
{
	input::InputManager inputManager;
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


	

public:
	App(glm::ivec2 screenSize, bool debugMode);
	~App();
	void run();
	Camera::Camera camera;
	float deltaTime = 0.0f;
	bool f12_button[2] = { {false }, { true } };
	input::InputManager& getInputManager();
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
int myMax(int a, int b);

#endif // !APP_H