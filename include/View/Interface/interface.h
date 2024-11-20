#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"
#include <View/vkImage/texture.h>
#include "View/Interface/Thumbs/thumbsManager.h"
#include <View/vkMesh/meshesManager.h>
#include "fileOperations/filesFinding.h"

struct DragDropData {
	char fullPath[256]; // Upewnij siê, ¿e to wystarczaj¹ca wielkoœæ bufora
	char name[64];      // Dostosuj rozmiary wed³ug potrzeby
};

class editor {

	

	Scene* scene;
	SceneObject* selectedObject = nullptr;
	ComponentType selectedComponentType = ComponentType::None;

	vkThumbs::ThumbsManager* miniatureManager;

	bool showComponentSelector = false;
	bool showModelSelector = false;
	bool isCut = false;
	bool isRMBPopup = false;

	std::filesystem::path baseFolder;
	std::filesystem::path currentFolder;

	//copy/cut paste
	std::string clipboard;
	

	vkImage::Texture* texture;

	void create_miniatures(vk::PhysicalDevice physicalDevice,
		vk::Device device,
		vk::CommandBuffer commandBuffer,
		vk::Queue queue,
		fileOperations::filesPaths textures,
		vkMesh::VertexMenagerie* meshes,
		vk::Format pictureFormat,
		vk::Format depthFormat,
		int number_of_models
		);

	public:
		editor(Scene* scene, std::string path, vkImage::TextureInputChunk info, fileOperations::filesPaths textures, vkMesh::VertexMenagerie* meshes,
			vk::Format pictureFormat,
			vk::Format depthFormat,
			int modelsNumber);
		~editor();
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, fileOperations::filesPaths models, fileOperations::filesPaths textures, vkMesh::MeshesManager* meshesManager, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		void AddComponent(ecs::ECS* ecs, ecs::Entity entity);
		void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
		void render_file_explorer();
		void rmb_click_render(std::filesystem::path path);
		void DisplaySceneObject(SceneObject* obj);
		void render_components_gui(fileOperations::filesPaths models, fileOperations::filesPaths textures, vkMesh::MeshesManager* meshesManager);
		void RemoveSceneObject(SceneObject* obj);
		void AddSceneObject(SceneObject* obj);
};