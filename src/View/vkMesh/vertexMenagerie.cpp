#include "View/vkMesh/vertexMenagerie.h"
#include <View/vkUtil/memory.h>

vkMesh::VertexMenagerie::VertexMenagerie() {
	indexOffset = 0;
}
vkMesh::VertexMenagerie::~VertexMenagerie() {
	logicalDevice.destroyBuffer(vertexBuffer.buffer);
	logicalDevice.destroyBuffer(indexBuffer.buffer);
	logicalDevice.freeMemory(vertexBuffer.bufferMemory);
	logicalDevice.freeMemory(indexBuffer.bufferMemory);
}
void vkMesh::VertexMenagerie::consume(uint64_t meshType, std::vector<float> data, std::vector<uint32_t> indices) {
	int indexCount = static_cast<int>(indices.size());
	int vertexCount = static_cast<int>(data.size()/8);
	int lastIndex = static_cast<int>(indexLump.size());

	firstIndices.insert(std::make_pair(meshType, lastIndex));
	indexCounts.insert(std::make_pair(meshType, indexCount));
	for (float attribute : data) {
		vertexLump.push_back(attribute);
	}
	for (uint32_t index : indices) {
		indexLump.push_back(index + indexOffset);
	}

	indexOffset += vertexCount;
}

void vkMesh::VertexMenagerie::finalize(FinalizationChunk finalizationChunk) {


	logicalDevice = finalizationChunk.logicalDevice;




	//make a staging buffer for vertices
	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = finalizationChunk.logicalDevice;
	inputChunk.physicalDevice = finalizationChunk.physicalDevice;
	inputChunk.size = sizeof(float) * vertexLump.size();
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent;
	Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);
	inputChunk.memoryAllocatet = vk::MemoryAllocateFlagBits::eDeviceAddress;
	//fill it with vertex data
	void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, vertexLump.data(), inputChunk.size);

	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//make the vertex buffer
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vertexBuffer = vkUtil::createBuffer(inputChunk);

	//copy to it
	vkUtil::copyBuffer(
		stagingBuffer, vertexBuffer, inputChunk.size,
		finalizationChunk.queue, finalizationChunk.commandBuffer
	);

	//destroy staging buffer
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);

	//make a staging buffer for indices
	inputChunk.size = sizeof(uint32_t) * indexLump.size();

	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;

	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent;
	stagingBuffer = vkUtil::createBuffer(inputChunk);

	//fill it with index data
	memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, indexLump.data(), inputChunk.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//make the vertex buffer
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	indexBuffer = vkUtil::createBuffer(inputChunk);

	//copy to it
	vkUtil::copyBuffer(
		stagingBuffer, indexBuffer, inputChunk.size,
		finalizationChunk.queue, finalizationChunk.commandBuffer
	);

	//destroy staging buffer
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
}
