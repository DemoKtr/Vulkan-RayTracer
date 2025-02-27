#pragma once
#include "config.h"

namespace vkInit {

	void create_attachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Format f, vk::ImageUsageFlagBits u, vk::Extent2D swapchainExten, vk::Image& image, vk::DeviceMemory& mem, vk::ImageView& view);

}