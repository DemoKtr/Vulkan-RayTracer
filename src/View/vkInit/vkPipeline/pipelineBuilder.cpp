#include "View/vkInit/vkPipeline/pipelineBuilder.h"
#include <View/vkInit/vkPipeline/shaders.h>
#include "View/RenderStructs/projection.h"
#include "View/vkUtil/renderpass.h"

vkInit::PipelineBuilder::PipelineBuilder(vk::Device device) {
	this->device = device;
	reset();

	//Some stages are fixed with sensible defaults and don't
	//need to be reconfigured
	configure_input_assembly();
	make_rasterizer_info();
	configure_multisampling();
	
	pipelineInfo.basePipelineHandle = nullptr;
}

vkInit::PipelineBuilder::~PipelineBuilder() {
	reset();
}

void vkInit::PipelineBuilder::reset() {
	pipelineInfo.flags = vk::PipelineCreateFlags();

	reset_vertex_format();
	reset_shader_modules();
	reset_descriptor_set_layouts();

	pushConstantCount = 0;
	pushConstantInfo.offset = 0;
	pushConstantInfo.size = 0;

}

void vkInit::PipelineBuilder::specify_vertex_format(
	vk::VertexInputBindingDescription bindingDescription,
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions) {

	this->bindingDescription = bindingDescription;
	this->attributeDescriptions = attributeDescriptions;

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &(this->bindingDescription);
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(this->attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = this->attributeDescriptions.data();

}

void vkInit::PipelineBuilder::reset_vertex_format() {

	vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

}

void vkInit::PipelineBuilder::set_overwrite_mode(bool mode) {
	overwrite = mode;
}

void vkInit::PipelineBuilder::set_renderpass(vk::RenderPass renderpass) {
	renderPass = renderpass;
}

vkInit::GraphicsPipelineOutBundle vkInit::PipelineBuilder::build(vk::Format swapchainFormat,vk::Format depthFormat){
	//Vertex Input
	configure_color_blending();

	pipelineInfo.pVertexInputState = &vertexInputInfo;

	//Input Assembly
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

	//Viewport and Scissor
	make_viewport_state();
	pipelineInfo.pViewportState = &viewportState;

	//Rasterizer
	pipelineInfo.pRasterizationState = &rasterizer;

	//Shader Modules
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();

	//Depth-Stencil is handled by depth attachment functions.

	//Multisampling
	pipelineInfo.pMultisampleState = &multisampling;

	//Color Blend
	pipelineInfo.pColorBlendState = &colorBlending;

	set_depth();
	//Pipeline Layout
	std::cout << "Create Pipeline Layout" << std::endl;
	vk::PipelineLayout pipelineLayout = make_pipeline_layout();
	pipelineInfo.layout = pipelineLayout;


	if (dynamicRendering) {
		vk::PipelineRenderingCreateInfoKHR pipeline_rendering_create_info; 
		pipeline_rendering_create_info.sType = vk::StructureType::ePipelineRenderingCreateInfoKHR,
		pipeline_rendering_create_info.colorAttachmentCount = 1,
		pipeline_rendering_create_info.pColorAttachmentFormats = &swapchainFormat;
		if (useDepthTest)
			pipeline_rendering_create_info.depthAttachmentFormat = vk::Format::eD32Sfloat;
		else
			pipeline_rendering_create_info.depthAttachmentFormat = vk::Format::eUndefined;
		pipelineInfo.renderPass = nullptr;
		pipelineInfo.pNext = &pipeline_rendering_create_info;
	}
		
	else pipelineInfo.renderPass = renderPass;
	

	//Make the Pipeline
	std::cout<<"Create Graphics Pipeline"<<std::endl;
	vk::Pipeline graphicsPipeline;
	try {
		graphicsPipeline = (device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
	}
	catch (vk::SystemError err) {
		std::cout<<"Failed to create Pipeline"<<std::endl;
	}

	GraphicsPipelineOutBundle output;
	output.layout = pipelineLayout;
	output.pipeline = graphicsPipeline;
	

	return output;
}

void vkInit::PipelineBuilder::add_descriptor_set_layout(vk::DescriptorSetLayout descriptorSetLayout) {
	descriptorSetLayouts.push_back(descriptorSetLayout);
}

void vkInit::PipelineBuilder::reset_descriptor_set_layouts(){
	descriptorSetLayouts.clear();
}

void vkInit::PipelineBuilder::reset_shader_modules() {
	if (vertexShader) {
		device.destroyShaderModule(vertexShader);
		vertexShader = nullptr;
	}
	if (fragmentShader) {
		device.destroyShaderModule(fragmentShader);
		fragmentShader = nullptr;
	}
	shaderStages.clear();
}

void vkInit::PipelineBuilder::configure_input_assembly() {
	inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
}
void vkInit::PipelineBuilder::configure_input_assembly(vk::PrimitiveTopology topology) {
	inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
	inputAssemblyInfo.topology = topology;
}

void vkInit::PipelineBuilder::set_depth() {
	if (useDepthTest) {
		depthState.flags = vk::PipelineDepthStencilStateCreateFlags();
		depthState.depthTestEnable = true;
		depthState.depthWriteEnable = true;
		depthState.depthCompareOp = vk::CompareOp::eLess;
		depthState.depthBoundsTestEnable = false;
		depthState.stencilTestEnable = false;
		pipelineInfo.pDepthStencilState = &depthState;
	}
	else {
		depthState.flags = vk::PipelineDepthStencilStateCreateFlags();
		depthState.depthTestEnable = false;
		depthState.depthWriteEnable = false;
		depthState.depthCompareOp = vk::CompareOp::eAlways;
		depthState.depthBoundsTestEnable = false;
		depthState.stencilTestEnable = false;
		pipelineInfo.pDepthStencilState = nullptr;
	}
}


vk::PipelineShaderStageCreateInfo vkInit::PipelineBuilder::make_shader_info(const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage) {
	vk::PipelineShaderStageCreateInfo shaderInfo = {};
	shaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	shaderInfo.stage = stage;
	shaderInfo.module = shaderModule;
	shaderInfo.pName = "main";
	return shaderInfo;
}
vk::PipelineViewportStateCreateInfo vkInit::PipelineBuilder::make_viewport_state() {

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapchainExtent.width;
	viewport.height = (float)swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset.x = 0.0f;
	scissor.offset.y = 0.0f;
	scissor.extent = swapchainExtent;

	viewportState.flags = vk::PipelineViewportStateCreateFlags();
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	return viewportState;
}

void vkInit::PipelineBuilder::make_rasterizer_info() {

	rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
	rasterizer.depthClampEnable = VK_FALSE; //discard out of bounds fragments, don't clamp them
	rasterizer.rasterizerDiscardEnable = VK_FALSE; //This flag would disable fragment output
	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
	rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.

}

void vkInit::PipelineBuilder::configure_multisampling() {

	multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

}

void vkInit::PipelineBuilder::configure_color_blending() {
	
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;

	colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = vk::LogicOp::eCopy;
	if (dynamicRendering) {
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
	}
	else {
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
	}
	
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

}

vk::PipelineLayout vkInit::PipelineBuilder::make_pipeline_layout() {

	/*
	typedef struct VkPipelineLayoutCreateInfo {
		VkStructureType                 sType;
		const void*                     pNext;
		VkPipelineLayoutCreateFlags     flags;
		uint32_t                        setLayoutCount;
		const VkDescriptorSetLayout*    pSetLayouts;
		uint32_t                        pushConstantRangeCount;
		const VkPushConstantRange*      pPushConstantRanges;
	} VkPipelineLayoutCreateInfo;
	*/

	vk::PipelineLayoutCreateInfo layoutInfo;
	layoutInfo.flags = vk::PipelineLayoutCreateFlags();

	layoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	layoutInfo.pSetLayouts = descriptorSetLayouts.data();

	
	
	layoutInfo.pushConstantRangeCount = pushConstantCount;
	layoutInfo.pPushConstantRanges = &pushConstantInfo;


	

	try {
		return device.createPipelineLayout(layoutInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to create pipeline layout!" << std::endl;
	}
}

void vkInit::PipelineBuilder::specify_vertex_shader(const char* filename) {

	if (vertexShader) {
		device.destroyShaderModule(vertexShader);
		vertexShader = nullptr;
	}

	std::cout << "Create vertex shader module" << std::endl;
	vertexShader = vkUtil::createModule(filename, device);
	vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
	shaderStages.push_back(vertexShaderInfo);
}

void vkInit::PipelineBuilder::specify_fragment_shader(const char* filename) {

	if (fragmentShader) {
		device.destroyShaderModule(fragmentShader);
		fragmentShader = nullptr;
	}

	std::cout << "Create fragment shader module" << std::endl;
	fragmentShader = vkUtil::createModule(filename, device);
	fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
	shaderStages.push_back(fragmentShaderInfo);
}

void vkInit::PipelineBuilder::specify_swapchain_extent(vk::Extent2D screen_size) {

	swapchainExtent = screen_size;
}




void vkInit::PipelineBuilder::clear_depth_attachment() {
	pipelineInfo.pDepthStencilState = nullptr;
}

void vkInit::PipelineBuilder::use_depth_test(bool is) {
	this->useDepthTest = is;
}

void vkInit::PipelineBuilder::setPushConstants(size_t size, int count) {
	pushConstantCount = count;
	pushConstantInfo.offset = 0;
	pushConstantInfo.size = size;
	pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;
}
