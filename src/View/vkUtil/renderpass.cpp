#include "View/vkUtil/renderpass.h"

vk::RenderPass vkUtil::create_imgui_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat){
	vk::AttachmentDescription attachment = {};




	attachment.flags = vk::AttachmentDescriptionFlags();
	attachment.samples = vk::SampleCountFlagBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::eClear;
	attachment.storeOp = vk::AttachmentStoreOp::eStore;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment.initialLayout = vk::ImageLayout::eUndefined;
	attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
	attachment.format = swapchainImageFormat;


	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass = {};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = nullptr;

	vk::RenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachment;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;



	vk::RenderPass imguiRenderPass = logicalDevice.createRenderPass(renderPassCreateInfo);


	return imguiRenderPass;
}
