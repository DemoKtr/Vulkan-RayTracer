#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"


class editor {

	Scene* scene;
	SceneObject* selectedObject = nullptr;
	public:
		editor(Scene* scene);
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		void DisplaySceneObject(SceneObject* obj);
		void editor::RemoveSceneObject(SceneObject* obj);
};