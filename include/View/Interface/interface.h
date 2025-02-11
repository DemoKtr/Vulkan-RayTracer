#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"
#include <View/vkImage/texture.h>
#include "View/Interface/Thumbs/thumbsManager.h"
#include <View/vkMesh/meshesManager.h>
#include "fileOperations/filesFinding.h"
#include "Scene/ECS/scripts/scriptCompiler.h"
#include "View/Interface/ImGuiRenderer/EditorCommands.h"
#include "MathFunctions/FastFourierTransform.h"



class editor {

	vkImGui::FilesExploresData filesExploresData;
	float leftPanelWidth = 200.0f;
	float rightPanelWidth = 200.0f;
	float bottomPanelHeight = 200.0f;
	float menuHeight = 0.0f;

	bool resizingLeft = false;
	bool resizingRight = false;
	bool resizingBottom = false;

	//math::FastFourierTransform* fft;
	Scene* scene;
	SceneObject* selectedObject = nullptr;
	ComponentType selectedComponentType = ComponentType::None;

	vkThumbs::ThumbsManager* miniatureManager;

	
	
	//bool isRMBPopup = false;

	
	
	



	void create_miniatures(vk::PhysicalDevice physicalDevice,
		vk::Device device,
		vk::CommandBuffer commandBuffer,
		vk::Queue queue,
		vk::Format pictureFormat,
		vk::Format depthFormat);

		
		
	public:
		

		editor(Scene* scene, std::string path,
			vkImage::TextureInputChunk info,
			vk::Format pictureFormat,
			vk::Format depthFormat);
		~editor();
		void render_editor(vk::CommandBuffer commandBuffer, std::vector<vkUtil::SwapChainFrame>& swapchainFrames, RenderObjects* objects, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode, float dt);
		

};