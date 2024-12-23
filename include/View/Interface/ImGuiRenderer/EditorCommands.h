#pragma once

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <settings.h>
#include "View/Interface/Thumbs/thumbsManager.h"
#include "Scene/sceneObject.h"
#include <View/vkMesh/meshesManager.h>
#include "View/Interface/Thumbs/thumbsManager.h"
#include "fileOperations/filesTypes.h"
namespace vkImGui {

	struct DragDropData {
		char fullPath[512]; // Upewnij siê, ¿e to wystarczaj¹ca wielkoœæ bufora
		char name[64];      // Dostosuj rozmiary wed³ug potrzeby
	};

	struct FilesExploresData {
		std::string clipboard;
		bool isCut = false;
		std::filesystem::path baseFolder;
		std::filesystem::path currentFolder;
	};

	static bool fileSaverIsOpen = false;
	static fileOperations::FileType::Type fileType = fileOperations::FileType::Type::Unknown;

	void render_editor(vkThumbs::ThumbsManager* miniatureManager, vkImGui::FilesExploresData& filesExploresData, SceneObject* root, SceneObject* &selectedObj, ComponentType& selectedComponentType, ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager);
	void render_console();
	void render_file_explorator(vkThumbs::ThumbsManager* miniatureManager, vkImGui::FilesExploresData& filesExploresData);
	void render_scenegraph(SceneObject* root, SceneObject*& selectedObject, ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager);

	void display_scene_object(SceneObject* &selectedObject, ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager, vkThumbs::ThumbsManager* miniatureManager, ComponentType& selectedComponentType);
	void rmb_click_render(std::filesystem::path path, vkImGui::FilesExploresData& filesExploresData);
	void rmb_click_render(SceneObject* root, ecs::ECS* ecs, vkMesh::MeshesManager* meshesManager);


}