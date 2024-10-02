#pragma once
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <fstream>

struct Buffer {
	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;
};

struct BufferInputChunk {
	size_t size;
	vk::BufferUsageFlags usage;
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::MemoryPropertyFlags memoryProperties;
};

enum class meshTypes {
	KITTY,
	DOG
};