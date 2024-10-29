#include "View/Interface/Thumbs/thumbRenderer.h"
#include "View/vkUtil/renderpass.h"
#include <View/vkInit/framebuffer.h>
#include <View/vkInit/vkPipeline/pipelineBuilder.h>
/*
Thumb::Thumb(vk::Device device, int width, int heigh, int n, int buffor, bool debugMode) {
	this->device = device;
	number_of_images = n;
	number_of_images_buffer = buffor;
}

void Thumb::create_renderpass(){
	renderPass = vkUtil::create_image_renderpass(device);
}

void Thumb::create_pipeline() {
	vkInit::PipelineBuilder pipelineBuilder(device);

	pipelineBuilder.set_overwrite_mode(false);
	pipelineBuilder.specify_vertex_shader("shaders/sky_vertex.spv");
	pipelineBuilder.specify_fragment_shader("shaders/sky_fragment.spv");
	vk::Extent2D extent{};
	extent.height = this->height;
	extent.width = this->witdth;
	pipelineBuilder.specify_swapchain_extent(extent);
	

	//vkInit::GraphicsPipelineOutBundle output = pipelineBuilder.build();


	pipelineBuilder.reset();

}

void Thumb::create_framebuffers(bool debug){

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &thumbsImageView;
		framebufferInfo.width = this->witdth;
		framebufferInfo.height = this->height;
		framebufferInfo.layers = number_of_images + number_of_images_buffer;

		try {
			framebuffer = device.createFramebuffer(framebufferInfo);

			if (debug) {
				std::cout << "Created framebuffer for models " << std::endl;
			}
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create framebuffer for models " << std::endl;
			}
		}

}

void Thumb::render(){

}
*/