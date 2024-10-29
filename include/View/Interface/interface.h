#pragma once
#include "config.h"
#include <View/vkUtil/frame.h>
#include "Scene/scene.h"
#include <View/vkMesh/meshTypes.h>
#include <View/vkImage/texture.h>


struct DragDropData {
	char fullPath[256]; // Upewnij siê, ¿e to wystarczaj¹ca wielkoœæ bufora
	char name[64];      // Dostosuj rozmiary wed³ug potrzeby
};

class editor {

	

	Scene* scene;
	SceneObject* selectedObject = nullptr;
	ComponentType selectedComponentType = ComponentType::None;
	bool showComponentSelector = false;
	bool showModelSelector = false;
	bool isCut = false;
	bool isRMBPopup = false;

	std::filesystem::path baseFolder;
	std::filesystem::path currentFolder;

	//copy/cut paste
	std::string clipboard;
	

	vkImage::Texture* texture;

	public:
		editor(Scene* scene,std::string path, vkImage::TextureInputChunk info);
		~editor();
		void render_editor(vk::CommandBuffer commandBuffer, vk::RenderPass imguiRenderPass, std::vector<vkUtil::SwapChainFrame> swapchainFrames, modelNames models, vk::Extent2D swapchainExtent, int numberOfFrame, bool debugMode);
		void DisplaySceneObject(SceneObject* obj);
		void RemoveSceneObject(SceneObject* obj);
		void AddSceneObject(SceneObject* obj);
		void AddComponent(ecs::ECS* ecs, ecs::Entity entity);
		void RemoveComponent(ecs::ECS* ecs, ecs::Entity entity, Component* component);
		void render_file_explorer();
		void rmb_click_render(std::filesystem::path path);
};