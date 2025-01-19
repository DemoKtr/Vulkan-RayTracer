#pragma once
#include "config.h"
#include "View/vkInit/vkPipeline/PipelineBuilder.h"
namespace vkInit {


	class MeshPipelineBuilder {
		
	public:

		MeshPipelineBuilder(vk::Device device);
		~MeshPipelineBuilder();

		void reset();




		void specify_task_shader(const char* filename);
		void specify_mesh_shader(const char* filename);
		void specify_fragment_shader(const char* filename);


		void specify_swapchain_extent(vk::Extent2D screen_size);
		void set_overwrite_mode(bool mode);

		GraphicsPipelineOutBundle build(vk::Format swapchainFormat, vk::Format depthFormat);

		void add_descriptor_set_layout(vk::DescriptorSetLayout descriptorSetLayout);

		void reset_descriptor_set_layouts();
		void clear_depth_attachment();
		

		void setPushConstants(size_t size, int count, size_t offset, vk::ShaderStageFlagBits shaderStage);
		void set_color_blending(bool is);

	private:
		vk::Device device;
		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		std::vector<vk::PushConstantRange> pushConstantInfo;


		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
		vk::ShaderModule taskShader = nullptr, meshShader = nullptr, fragmentShader = nullptr;
		vk::PipelineShaderStageCreateInfo taskShaderInfo, meshShaderInfo,fragmentShaderInfo;

		vk::Extent2D swapchainExtent;
		vk::Viewport viewport = {};
		vk::Rect2D scissor = {};
		vk::PipelineViewportStateCreateInfo viewportState = {};

		vk::PipelineRasterizationStateCreateInfo rasterizer = {};

		vk::PipelineDepthStencilStateCreateInfo depthState;
		std::unordered_map<uint32_t, vk::AttachmentDescription> attachmentDescriptions;
		std::unordered_map<uint32_t, vk::AttachmentReference> attachmentReferences;
		std::vector<vk::AttachmentDescription> flattenedAttachmentDescriptions;
		std::vector<vk::AttachmentReference> flattenedAttachmentReferences;

		vk::PipelineMultisampleStateCreateInfo multisampling = {};

		vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
		vk::PipelineColorBlendStateCreateInfo colorBlending = {};

		std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

		vk::RenderPass renderPass;
		bool overwrite, useDepthTest, useColorBlending;

		void reset_vertex_format();

		void reset_shader_modules();


		vk::PipelineShaderStageCreateInfo make_shader_info(
			const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);

		vk::PipelineViewportStateCreateInfo make_viewport_state();

		void make_rasterizer_info();

		void configure_multisampling();

		void configure_color_blending();

		vk::PipelineLayout make_pipeline_layout();
		void configure_input_assembly();

	};
}