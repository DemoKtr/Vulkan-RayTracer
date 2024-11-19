#pragma once
#include "View/Interface/Thumbs/iconsThumbs.h"
#include "View/Interface/Thumbs/meshesThumbs.h"
#include "View/Interface/Thumbs/texturesThumbs.h"


namespace vkThumbs {

	struct ThumbsManagerInput {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vkImage::TexturesNames textures;
		vkMesh::VertexMenagerie* meshes;
		vk::Format pictureFormat;
		vk::Format depthFormat;
		int number_of_models;

	};

	class ThumbsManager {
		vk::Device logicalDevice;

		IconThumbs* iconsThums;
		TextureThumbs* texturesThumbs;
		MeshesTumbs* meshesThumbs;

		

		vk::DescriptorPool descriptorPool;
		vk::DescriptorSetLayout descriptorLayout;

		void make_descriptor_layout();


	public :
		ThumbsManager(ThumbsManagerInput info);
		~ThumbsManager();
		ImTextureID get_folder_icon();
		ImTextureID get_header_icon();
		ImTextureID get_source_icon();
		ImTextureID get_text_icon();
		ImTextureID get_file_icon();
		ImTextureID get_mesh_icon(int index);
		ImTextureID get_texture_icon(int index);
	};
}

