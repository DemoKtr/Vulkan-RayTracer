#pragma once
#include "config.h"
#include "vertex.h"


struct  FinalizationChunk {
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;

};



class VertexMenagerie
{
	int indexOffset;
	
	vk::Device logicalDevice;
	std::vector<vkGeometry::Vertex> staticVertex;
	std::vector<float> vertexLump;
	std::vector<uint32_t> indexLump;


public:
	VertexMenagerie();
	~VertexMenagerie();
	void consume(meshTypes meshType, std::vector<vkGeometry::Vertex> data, std::vector<uint32_t> indices);
	

	void finalize(FinalizationChunk finalizationChink);
	Buffer vertexBuffer, indexBuffer;
	std::unordered_map<meshTypes, int> firstIndices;
	std::unordered_map<meshTypes, int> indexCounts;

};
