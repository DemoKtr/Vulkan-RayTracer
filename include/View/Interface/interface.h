#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"
#include <View/vkMesh/meshTypes.h>
#include <View/vkImage/texture.h>
#include "View/Interface/Thumbs/thumbsManager.h"
#include <View/vkMesh/meshesManager.h>

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
		vkImage::TexturesNames textures,
		vkMesh::VertexMenagerie* meshes,
		vk::Format pictureFormat,
		vk::Format depthFormat,
		int number_of_models
		);

	public:
		editor(Scene* scene, std::string path, vkImage::TextureInputChunk info, vkImage::TexturesNames textures, vkMesh::VertexMenagerie* meshes,
			vk::Format pictureFormat,
			vk::Format depthFormat,
			int modelsNumber);
		~editor();
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, modelNames models, vkImage::TexturesNames textures, vkMesh::MeshesManager* meshesManager,vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		void DisplaySceneObject(SceneObject* obj);
		void RemoveSceneObject(SceneObject* obj);
		void AddSceneObject(SceneObject* obj);
		void AddComponent(ecs::ECS* ecs, ecs::Entity entity);
		void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
		void render_file_explorer();
		void rmb_click_render(std::filesystem::path path);

		void render_components_gui(modelNames models, vkImage::TexturesNames textures, vkMesh::MeshesManager* meshesManager);
};