#pragma once
#include "config.h"

namespace vkUtil {
	vk::RenderPass create_imgui_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat);
	vk::RenderPass create_image_renderpass(vk::Device logicalDevice);
	vk::RenderPass create_postprocess_renderpass(vk::Device logicalDevice,vk::Format swapchainImageFormat, vk::Format depthImageFormat);
}