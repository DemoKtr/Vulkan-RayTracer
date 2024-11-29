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


struct ScriptsFiels {
	fileOperations::filesPaths& cpp;
	fileOperations::filesPaths& dll;
	int scriptsCounter{0};

	// Konstruktor inicjalizuj¹cy referencje
	
};




class editor {

	vkImGui::FilesExploresData filesExploresData;
	float leftPanelWidth = 200.0f;
	float rightPanelWidth = 200.0f;
	float bottomPanelHeight = 200.0f;
	float menuHeight = 0.0f;

	bool resizingLeft = false;
	bool resizingRight = false;
	bool resizingBottom = false;


	Scene* scene;
	SceneObject* selectedObject = nullptr;
	ComponentType selectedComponentType = ComponentType::None;

	vkThumbs::ThumbsManager* miniatureManager;

	
	
	//bool isRMBPopup = false;

	
	
	

	vkImage::Texture* texture;

	void create_miniatures(vk::PhysicalDevice physicalDevice,
		vk::Device device,
		vk::CommandBuffer commandBuffer,
		vk::Queue queue,
		fileOperations::filesPaths models,
		fileOperations::filesPaths textures,
		vkMesh::VertexMenagerie* meshes,
		vk::Format pictureFormat,
		vk::Format depthFormat,
		int number_of_models
		);
	ScriptsFiels scriptsFiles;
	public:
		

		editor(Scene* scene, std::string path, vkImage::TextureInputChunk info,ScriptsFiels scripts ,fileOperations::filesPaths models, fileOperations::filesPaths textures, vkMesh::VertexMenagerie* meshes,
			vk::Format pictureFormat,
			vk::Format depthFormat,
			int modelsNumber);
		~editor();
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, fileOperations::filesPaths models, fileOperations::filesPaths textures, vkMesh::MeshesManager* meshesManager, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		

};