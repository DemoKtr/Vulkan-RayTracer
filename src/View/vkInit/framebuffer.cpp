#include "View/vkInit/framebuffer.h"

void vkInit::make_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug){
	/*
	for (int i = 0; i < frames.size(); ++i) {

		std::vector<vk::ImageView> attachments = {
			frames[i].mainimageView
			//, frames[i].gbuffer.position.view, frames[i].gbuffer.normal.view, frames[i].gbuffer.albedo.view
		};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = inputChunk.renderpass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = inputChunk.swapchainExtent.width;
		framebufferInfo.height = inputChunk.swapchainExtent.height;
		framebufferInfo.layers = 1;

		try {
			frames[i].imguiFrameBuffer = inputChunk.device.createFramebuffer(framebufferInfo);

			if (debug) {
				std::cout << "Created framebuffer for frame " << i << std::endl;
			}
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create framebuffer for frame " << i << std::endl;
			}
		}

	}
	*/
}

void vkInit::make_postprocess_framebuffer(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {
	/*
	for (int i = 0; i < frames.size(); ++i) {
		
		std::vector<vk::ImageView> attachments = {
			frames[i].mainimageView, frames[i].depthBufferView,
		};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = inputChunk.renderpass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = inputChunk.swapchainExtent.width;
		framebufferInfo.height = inputChunk.swapchainExtent.height;
		framebufferInfo.layers = 1;

		try {
			frames[i].postProcessFrameBuffer = inputChunk.device.createFramebuffer(framebufferInfo);

			if (debug) {
				std::cout << "Created framebuffer for frame " << i << std::endl;
			}
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create framebuffer for frame " << i << std::endl;
			}
		}

	}
	*/
}
