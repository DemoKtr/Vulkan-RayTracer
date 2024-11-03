#pragma once
#include "config.h"
#include <View/vkVertexData/vertex.h>

namespace vkMesh {
	struct  FinalizationChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::Queue queue;
		vk::CommandBuffer commandBuffer;

	};

	class VertexMenagerie
	{
		int indexOffset;
		bool isAnimated{ false };
		vk::Device logicalDevice;
		



	public:
		VertexMenagerie();
		~VertexMenagerie();
		void consume(int meshType, std::vector<float> data, std::vector<uint32_t> indices);
		std::vector<float> vertexLump;
		std::vector<uint32_t> indexLump;
		std::vector<vkGeometry::Vertex> vertices;
		void finalize(FinalizationChunk finalizationChunk);
		Buffer vertexBuffer, indexBuffer;
		std::unordered_map<int, int> firstIndices;
		std::unordered_map<int, int> indexCounts;


	};
}