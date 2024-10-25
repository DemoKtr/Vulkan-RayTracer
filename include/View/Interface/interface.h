#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"
#include <View/vkMesh/meshTypes.h>


class editor {

	Scene* scene;
	SceneObject* selectedObject = nullptr;
	ComponentType selectedComponentType = ComponentType::None;
	bool showComponentSelector = false;
	bool showModelSelector = false;
	public:
		editor(Scene* scene);
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, modelNames models, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		void DisplaySceneObject(SceneObject* obj);
		void RemoveSceneObject(SceneObject* obj);
		void AddSceneObject(SceneObject* obj);
		void AddComponent(ecs::ECS* ecs, ecs::Entity entity);
		void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
};