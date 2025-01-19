#include "View/vkInit/vkPipeline/meshPipelineBuilder.h"
#include <View/vkInit/vkPipeline/shaders.h>
#include "View/RenderStructs/projection.h"
#include "View/vkUtil/renderpass.h"

vkInit::MeshPipelineBuilder::MeshPipelineBuilder(vk::Device device) {
	this->device = device;
	reset();

	//Some stages are fixed with sensible defaults and don't
	//need to be reconfigured
	configure_input_assembly();
	make_rasterizer_info();
	configure_multisampling();

	pipelineInfo.basePipelineHandle = nullptr;
}

vkInit::MeshPipelineBuilder::~MeshPipelineBuilder() {
	this->reset();
}

void vkInit::MeshPipelineBuilder::reset() {
	pipelineInfo.flags = vk::PipelineCreateFlags();

	reset_shader_modules();
	reset_descriptor_set_layouts();

	pushConstantInfo.clear();

}



vkInit::GraphicsPipelineOutBundle vkInit::MeshPipelineBuilder::build(vk::Format swapchainFormat, vk::Format depthFormat) {
	//Vertex Input
	configure_color_blending();

	pipelineInfo.pVertexInputState = nullptr;

	//Input Assembly
	pipelineInfo.pInputAssemblyState = nullptr;

	//Viewport and Scissor
	make_viewport_state();
	pipelineInfo.pViewportState = &viewportState;

	//Rasterizer
	pipelineInfo.pRasterizationState = &rasterizer;

	//Shader Modules
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();


	pipelineInfo.pMultisampleState = &multisampling;

	//Color Blend
	//
	pipelineInfo.pColorBlendState = &colorBlending;
	depthState.flags = vk::PipelineDepthStencilStateCreateFlags();
	depthState.depthTestEnable = false;
	depthState.depthWriteEnable = false;
	depthState.depthCompareOp = vk::CompareOp::eLess;
	depthState.depthBoundsTestEnable = false;
	depthState.stencilTestEnable = false;
	pipelineInfo.pDepthStencilState = &depthState;

	//Pipeline Layout
	std::cout << "Create Pipeline Layout" << std::endl;
	vk::PipelineLayout pipelineLayout = make_pipeline_layout();
	pipelineInfo.layout = pipelineLayout;


	vk::PipelineRenderingCreateInfoKHR pipeline_rendering_create_info;
	pipeline_rendering_create_info.sType = vk::StructureType::ePipelineRenderingCreateInfoKHR,
	pipeline_rendering_create_info.colorAttachmentCount = 1,
	pipeline_rendering_create_info.pColorAttachmentFormats = &swapchainFormat;
	pipeline_rendering_create_info.depthAttachmentFormat = vk::Format::eD32Sfloat;
	pipelineInfo.renderPass = nullptr;
	pipelineInfo.pNext = &pipeline_rendering_create_info;



	//Make the Pipeline
	std::cout << "Create Graphics Pipeline" << std::endl;
	vk::Pipeline graphicsPipeline;
	try {
		graphicsPipeline = (device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to create Pipeline" << std::endl;
	}

	GraphicsPipelineOutBundle output;
	output.layout = pipelineLayout;
	output.pipeline = graphicsPipeline;


	return output;
}

void vkInit::MeshPipelineBuilder::add_descriptor_set_layout(vk::DescriptorSetLayout descriptorSetLayout) {
	descriptorSetLayouts.push_back(descriptorSetLayout);
}

void vkInit::MeshPipelineBuilder::reset_descriptor_set_layouts() {
	descriptorSetLayouts.clear();
}

void vkInit::MeshPipelineBuilder::clear_depth_attachment() {
	pipelineInfo.pDepthStencilState = nullptr;
}

void vkInit::MeshPipelineBuilder::setPushConstants(size_t size, int count,size_t offset ,vk::ShaderStageFlagBits shaderStage) {
	vk::PushConstantRange pushInfo;

	pushInfo.offset = offset;
	pushInfo.size = size;
	pushInfo.stageFlags = shaderStage;
	pushConstantInfo.push_back(pushInfo);
}

void vkInit::MeshPipelineBuilder::set_color_blending(bool is) {
	useColorBlending = is;
}

void vkInit::MeshPipelineBuilder::reset_shader_modules() {
	if (taskShader) {
		device.destroyShaderModule(taskShader);
		taskShader = nullptr;
	}
	if (meshShader) {
		device.destroyShaderModule(meshShader);
		meshShader = nullptr;
	}
	if (fragmentShader) {
		device.destroyShaderModule(fragmentShader);
		fragmentShader = nullptr;
	}
	shaderStages.clear();
}

vk::PipelineShaderStageCreateInfo vkInit::MeshPipelineBuilder::make_shader_info(const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage) {
	vk::PipelineShaderStageCreateInfo shaderInfo = {};
	shaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	shaderInfo.stage = stage;
	shaderInfo.module = shaderModule;
	shaderInfo.pName = "main";
	return shaderInfo;
}

vk::PipelineViewportStateCreateInfo vkInit::MeshPipelineBuilder::make_viewport_state() {
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

void vkInit::MeshPipelineBuilder::make_rasterizer_info() {
	rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
	rasterizer.depthClampEnable = VK_FALSE; //discard out of bounds fragments, don't clamp them
	rasterizer.rasterizerDiscardEnable = VK_FALSE; //This flag would disable fragment output
	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = vk::CullModeFlagBits::eNone;
	rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
	rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.
}

void vkInit::MeshPipelineBuilder::configure_multisampling() {
	multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
}

void vkInit::MeshPipelineBuilder::configure_color_blending() {
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	if (!useColorBlending) {
		colorBlendAttachment.blendEnable = VK_FALSE;
	}
	else {
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
		colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
		colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
	}


	colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = vk::LogicOp::eCopy;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	
	

	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;
}

vk::PipelineLayout vkInit::MeshPipelineBuilder::make_pipeline_layout() {
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



	layoutInfo.pushConstantRangeCount = pushConstantInfo.size();
	layoutInfo.pPushConstantRanges = pushConstantInfo.data();




	try {
		return device.createPipelineLayout(layoutInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to create pipeline layout!" << std::endl;
	}
}

void vkInit::MeshPipelineBuilder::configure_input_assembly() {
	inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
	
}



void vkInit::MeshPipelineBuilder::specify_task_shader(const char* filename) {

	if (taskShader) {
		device.destroyShaderModule(taskShader);
		taskShader = nullptr;
	}

	std::cout << "Create Task shader module" << std::endl;
	taskShader = vkUtil::createModule(filename, device);
	taskShaderInfo = make_shader_info(taskShader, vk::ShaderStageFlagBits::eTaskEXT);
	shaderStages.push_back(taskShaderInfo);
}

void vkInit::MeshPipelineBuilder::specify_mesh_shader(const char* filename) {
	if (meshShader) {
		device.destroyShaderModule(meshShader);
		meshShader = nullptr;
	}

	std::cout << "Create Mesh shader module" << std::endl;
	meshShader = vkUtil::createModule(filename, device);
	meshShaderInfo = make_shader_info(meshShader, vk::ShaderStageFlagBits::eMeshEXT);
	shaderStages.push_back(meshShaderInfo);
}

void vkInit::MeshPipelineBuilder::specify_fragment_shader(const char* filename) {
	if (fragmentShader) {
		device.destroyShaderModule(fragmentShader);
		fragmentShader = nullptr;
	}

	std::cout << "Create fragment shader module" << std::endl;
	fragmentShader = vkUtil::createModule(filename, device);
	fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
	shaderStages.push_back(fragmentShaderInfo);
}

void vkInit::MeshPipelineBuilder::specify_swapchain_extent(vk::Extent2D screen_size) {
	swapchainExtent = screen_size;
}

void vkInit::MeshPipelineBuilder::set_overwrite_mode(bool mode) {
	overwrite = mode;
}


