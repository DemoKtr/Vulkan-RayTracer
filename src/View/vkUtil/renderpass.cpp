#include "View/vkUtil/renderpass.h"
#include "View/vkImage/image.h"
vk::RenderPass vkUtil::create_imgui_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat){
	vk::AttachmentDescription attachment = {};




	attachment.flags = vk::AttachmentDescriptionFlags();
	attachment.samples = vk::SampleCountFlagBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::eDontCare;
	attachment.storeOp = vk::AttachmentStoreOp::eStore;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
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

vk::RenderPass vkUtil::create_image_renderpass(vk::Device logicalDevice) {
	
	vk::AttachmentDescription attachment = {};

	attachment.flags = vk::AttachmentDescriptionFlags();
	attachment.samples = vk::SampleCountFlagBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::eClear;
	attachment.storeOp = vk::AttachmentStoreOp::eStore;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment.initialLayout = vk::ImageLayout::eUndefined;
	attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachment.format = vk::Format::eR8G8B8Uint;


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



	vk::RenderPass renderPass = logicalDevice.createRenderPass(renderPassCreateInfo);


	return renderPass;
}

vk::RenderPass vkUtil::create_postprocess_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat, vk::Format depthImageFormat) {
	
	std::array<vk::AttachmentDescription, 2> attachments;


	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].format = swapchainImageFormat;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eUndefined;
	attachments[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

	

	attachments[1].flags = vk::AttachmentDescriptionFlags();
	attachments[1].format = depthImageFormat;
	attachments[1].samples = vk::SampleCountFlagBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[1].initialLayout = vk::ImageLayout::eUndefined;
	attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::SubpassDescription subpass = {};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;


	std::array<vk::SubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;


	vk::RenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = attachments.size();
	renderPassCreateInfo.pAttachments = attachments.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 2;
	renderPassCreateInfo.pDependencies = dependencies.data();


	vk::RenderPass postprocess = logicalDevice.createRenderPass(renderPassCreateInfo);


	return postprocess;


}
