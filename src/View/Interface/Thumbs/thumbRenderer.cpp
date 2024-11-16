#include "View/Interface/Thumbs/thumbRenderer.h"
#include "View/vkUtil/renderpass.h"
#include <View/vkInit/framebuffer.h>
#include <View/vkInit/vkPipeline/pipelineBuilder.h>
#include <View/vkMesh/vertexFormat.h>
#include <View/RenderStructs/projection.h>
#include <View/vkInit/descrpitors.h>



ThumbRenderer::ThumbRenderer(ThumbRendererInput input,bool debugMode) {
	commandBuffer = input.commandBuffer;
	depthFormat = input.depthFormat;
	device = input.device;
	height = input.heigh;
	meshes = input.meshes;
	number_of_images = input.number_of_models;
	physicalDevice = input.physicalDevice;
	imageFormat = input.pictureFormat;
	queue = input.queue;
	width = input.width;



	vk::Extent2D ext;
	ext.width = width;
	ext.height = height;
	swapchainExten = ext;
	vpData = vkRenderStructs::getViewProjectionMatrix(width,height);

	create_descriptors_set_layout();

	create_renderpass();

	create_pipeline();

	create_images();

	
	create_framebuffers(debugMode);

	make_assets();

	render(debugMode);
	
	
}

void ThumbRenderer::create_renderpass() {
	std::array<vk::AttachmentDescription, 2> attachments;

	
	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].format = vk::Format::eR8G8B8A8Unorm;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eUndefined;
	attachments[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;



	attachments[1].flags = vk::AttachmentDescriptionFlags();
	attachments[1].format = depthFormat;
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


	renderPass = device.createRenderPass(renderPassCreateInfo);

}

void ThumbRenderer::create_pipeline(){
	create_renderpass();

	vkInit::PipelineBuilder pipelineBuilder(device);
	
	
	pipelineBuilder.set_overwrite_mode(true);
	pipelineBuilder.specify_vertex_format(
		vkMesh::getVertexInputBindingDescription(),
		vkMesh::getVertexInputAttributeDescription());
	pipelineBuilder.specify_vertex_shader("resources/shaders/miniatureRenderer/vert.spv");
	pipelineBuilder.specify_fragment_shader("resources/shaders/miniatureRenderer/frag.spv");
	pipelineBuilder.specify_swapchain_extent(swapchainExten);
	pipelineBuilder.set_renderpass(renderPass);
	pipelineBuilder.clear_depth_attachment();
	pipelineBuilder.add_descriptor_set_layout(descriptorLayout);
	pipelineBuilder.use_depth_test(true);
	pipelineBuilder.use_projection_matrix(true);
	pipelineBuilder.use_view_matrix(true);

	vkInit::GraphicsPipelineOutBundle output = pipelineBuilder.build(imageFormat, depthFormat);

	pipelineLayout = output.layout;

	pipeline = output.pipeline;

	pipelineBuilder.reset();
	
}

void ThumbRenderer::create_framebuffers(bool debug) {
		
	for (uint32_t i = 0; i < number_of_images; ++i) {
		std::vector<vk::ImageView> attachments = {
			imageView[i], depthImageView[i]
		};
		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		try {
			framebuffers.push_back(device.createFramebuffer(framebufferInfo));

			if (debug) {
				std::cout << "Created model " << i << "  framebuffer" << std::endl;
			}
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create framebuffer for frame " << i << std::endl;
			}
		}

	}

	

	
}

void ThumbRenderer::render(bool debugMode) {
	
	vk::CommandBufferBeginInfo beginInfo = {};

	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording command buffer!" << std::endl;
		}
	}

	

	uint32_t startInstance = 0;
	//Triangles


	vk::ClearValue backgroundColor;
	std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f };
	backgroundColor.color = vk::ClearColorValue(color);
	vk::ClearValue depthColorValue;
	depthColorValue.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> colorVector = { {backgroundColor,depthColorValue} };
	
	
	for (int i = 0; i < number_of_images; ++i) {
		vk::RenderPassBeginInfo renderpassInfo = {};
		renderpassInfo.renderPass = renderPass;
		renderpassInfo.renderArea.offset.x = 0;
		renderpassInfo.renderArea.offset.y = 0;
		renderpassInfo.renderArea.extent.width = width;
		renderpassInfo.renderArea.extent.height = height;
		renderpassInfo.clearValueCount = colorVector.size();
		renderpassInfo.pClearValues = colorVector.data();
		renderpassInfo.framebuffer = framebuffers[i];
		commandBuffer.beginRenderPass(&renderpassInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
		commandBuffer.pushConstants(pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vkRenderStructs::ViewProjectionData), &vpData);
		prepare_scene();
		int indexCount = meshes->indexCounts.find(i)->second;
		int firstIndex = meshes->firstIndices.find(i)->second;
		universalTexture->useTexture(commandBuffer, pipelineLayout,0);
		commandBuffer.drawIndexed(indexCount, 1, firstIndex, 0, startInstance);
		startInstance++;
		commandBuffer.endRenderPass();

	
		
	// Przyk³ad zmiany layout obrazu przed jego u¿yciem w shaderze:
	vk::ImageMemoryBarrier barrier = {};
	barrier.srcAccessMask = {};
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	barrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image[i];  // Twoje Ÿród³o obrazu
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eFragmentShader,
		vk::DependencyFlags(), {}, {}, barrier);
	
	}

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	queue.submit(1, &submitInfo, nullptr);
	queue.waitIdle();

}

void ThumbRenderer::create_images() {

	for (int i = 0; i < number_of_images; ++i) {
	
		vkImage::ImageInputChunk imageInput;
		imageInput.logicalDevice = device;
		imageInput.physicalDevice = physicalDevice;
		imageInput.width = width;
		imageInput.height = height;
		imageInput.format = vk::Format::eR8G8B8A8Unorm;
		imageInput.arrayCount = 1;
		imageInput.tiling = vk::ImageTiling::eOptimal;
		imageInput.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled;
		imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
		image.push_back(make_image(imageInput));
		imageMemory.push_back(make_image_memory(imageInput, image[i]));
		imageView.push_back(vkImage::make_image_view(device, image[i], vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, 1));
	
		imageInput.format = depthFormat;
		imageInput.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		depthImage.push_back(make_image(imageInput));
		depthImageMemory.push_back(make_image_memory(imageInput, depthImage[i]));
		depthImageView.push_back(vkImage::make_image_view(device, depthImage[i], depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1));

	}
}

void ThumbRenderer::create_descriptors_set_layout() {
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 1;
	bindings.indices.push_back(0);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);

	descriptorLayout = vkInit::make_descriptor_set_layout(device, bindings);
	descriptorPool = vkInit::make_descriptor_pool(device, number_of_images, bindings);

}

void ThumbRenderer::make_assets() {
	std::string str = std::string(PROJECT_DIR) + "\\core\\u.png";

	vkImage::TextureInputChunk info;
	info.commandBuffer = commandBuffer;
	info.descriptorPool = descriptorPool;
	info.filenames = str.c_str();
	info.layout = descriptorLayout;
	info.logicalDevice = device;
	info.physicalDevice = physicalDevice;
	info.queue = queue;
	
	universalTexture = new vkImage::Texture(info);
	
}

ThumbRendererOutput ThumbRenderer::get_meshes_images() {

	
	ThumbRendererOutput output;
	output.image = image;
	output.imageMemory = imageMemory;
	output.imageView = imageView;

	
	return output;
}



void ThumbRenderer::render_objects(int objectType, uint32_t& startInstance) {
	int indexCount = meshes->indexCounts.find(objectType)->second;
	int firstIndex = meshes->firstIndices.find(objectType)->second;
	//materials[objectType]->useTexture(commandBuffer, layout);

	universalTexture->useTexture(commandBuffer, pipelineLayout, 0);
	commandBuffer.drawIndexed(indexCount, 1, firstIndex, 0, startInstance);

	startInstance += 1;
}

void ThumbRenderer::prepare_scene() {
	vk::Buffer vertexBuffers[] = { meshes->vertexBuffer.buffer };
	vk::DeviceSize offets[] = { 0 };
	commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offets);
	commandBuffer.bindIndexBuffer(meshes->indexBuffer.buffer, 0, vk::IndexType::eUint32);
}
