#pragma once
#include "config.h"

namespace vkUtil {
	vk::RenderPass create_imgui_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat);
}