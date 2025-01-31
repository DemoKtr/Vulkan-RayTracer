#pragma once
#ifndef DESCRIPTOR_MANAGER_H
#define DESCRIPTOR_MANAGER_H
#include "config.h"
#include <mutex>
#include "View/vkParticle/Particle.h"
#include <View/vkUtil/memory.h>
namespace vkUtil {
	struct DescriptorDataInput {
	
		vk::BufferUsageFlagBits usage;
		bool canTransfer = false;
		size_t count;
	};

	template <typename T>
	class DescriptorData {

	public:
		DescriptorData(vk::PhysicalDevice physicalDevice, vk::Device device) : physicalDevice(physicalDevice), logicalDevice(device) {}
		~DescriptorData(){ this->destroy(); }
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		// Dane u¿ytkownika (np. particleData)
		std::vector<T> data;

		// Bufor dla danych
		Buffer dataBuffer;
		void* dataWriteLocation = nullptr;

		vk::DescriptorBufferInfo descriptorInfo;


		void make_descriptors_resources(DescriptorDataInput inputChunk) {
			BufferInputChunk input;
			input.logicalDevice = logicalDevice;
			if (inputChunk.canTransfer)
				input.usage = inputChunk.usage | vk::BufferUsageFlagBits::eTransferDst;
			else input.usage = inputChunk.usage;
			input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
			input.physicalDevice = physicalDevice;
			input.size = inputChunk.count * sizeof(T);
			dataBuffer = createBuffer(input);
			dataWriteLocation = logicalDevice.mapMemory(dataBuffer.bufferMemory, 0, inputChunk.count * sizeof(T));
			
			data.reserve(inputChunk.count);
			for (uint32_t i = 0; i < inputChunk.count; ++i) {
				data.push_back(T());
			}
			
			
			
			descriptorInfo.buffer = dataBuffer.buffer;
			descriptorInfo.offset = 0;
			descriptorInfo.range = inputChunk.count * sizeof(T);
		}
		
		void destroy() {
			logicalDevice.unmapMemory(dataBuffer.bufferMemory);
			logicalDevice.freeMemory(dataBuffer.bufferMemory);
			logicalDevice.destroyBuffer(dataBuffer.buffer);
		}

	};
}
#endif // DESCRIPTOR_MANAGER_H