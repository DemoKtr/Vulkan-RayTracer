#pragma once
#include <View/Interface/Thumbs/thumbs.h>
#include <imgui.h>
#include "View/vkMesh/vertexMenagerie.h"
#include "fileOperations/filesFinding.h"
namespace vkThumbs {

	struct MeshesThumbInput{
		vkMesh::VertexMenagerie* meshes;
		fileOperations::filesPaths models;
		vk::Format pictureFormat;
		vk::Format depthFormat;
		int width;
		int heigh;
	};

	class MeshesTumbs : public Thumb {
		vk::Sampler sampler;
		std::vector<vk::Image> image;
		std::vector<vk::DeviceMemory> imageMemory;
		std::vector<vk::ImageView>  imageView;

		std::vector<vk::DescriptorSet> descriptorsSet;
	public:
		MeshesTumbs(ThumbInput input, MeshesThumbInput meshesInput);
		~MeshesTumbs();
		ImTextureID get_texture_icon(int index);
		void make_descrptor_set(MeshesThumbInput meshesInput);

	};

}