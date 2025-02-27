#pragma once
#include "config.h"

namespace vkInit {
	struct GraphicsPipelineOutBundle {
		vk::PipelineLayout layout;
		vk::Pipeline pipeline;
	};

	class PipelineBuilder {
		
	public:
		bool dynamicRendering = false;
		PipelineBuilder(vk::Device device);
		~PipelineBuilder();

		void reset();

		/**
			Configure the vertex input stage.

			\param bindingDescription describes the vertex inputs (ie. layouts)
			\param attributeDescriptions describes the attributes
			\returns the vertex input stage creation info

		*/
		void specify_vertex_format(
			vk::VertexInputBindingDescription bindingDescription,
			std::vector<vk::VertexInputAttributeDescription> attributeDescriptions);

		void specify_vertex_shader(const char* filename);

		void specify_fragment_shader(const char* filename);

		void specify_swapchain_extent(vk::Extent2D screen_size);



		void set_overwrite_mode(bool mode);
		void set_renderpass(vk::RenderPass renderpass);

		GraphicsPipelineOutBundle build(vk::Format swapchainFormat, vk::Format depthFormat);

		void add_descriptor_set_layout(vk::DescriptorSetLayout descriptorSetLayout);

		void reset_descriptor_set_layouts();
		void clear_depth_attachment();

		void use_depth_test(bool is);
		

		void setPushConstants(size_t size, int count);
		void set_color_blending(bool is);
		void make_rasterizer_info(vk::CullModeFlagBits bit);
	private:
		vk::Device device;
		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		vk::PushConstantRange pushConstantInfo;
		int pushConstantCount = 0;
		vk::VertexInputBindingDescription bindingDescription;
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
		vk::ShaderModule vertexShader = nullptr, fragmentShader = nullptr;
		vk::PipelineShaderStageCreateInfo vertexShaderInfo, fragmentShaderInfo;

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


		/**
			Make an attachment description

			\param format the image format for the underlying resource
			\param finalLayout the expected final layout after implicit transition (acquisition)
			\returns a description of the corresponding attachment
		*/


		/**
			\returns Make a renderpass attachment reference


		/**
			set up the input assembly stage
		*/
		void configure_input_assembly();
		void configure_input_assembly(vk::PrimitiveTopology topology);


		void set_depth();
		
		
		/**
			Configure a programmable shader stage.

			\param shaderModule the compiled shader module
			\param stage the shader stage which the module is for
			\returns the shader stage creation info
		*/
		vk::PipelineShaderStageCreateInfo make_shader_info(
			const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);

		/**
			Configure the pipeline's viewport stage.

			\param viewport the viewport specification
			\param scissor the scissor rectangle to apply
			\returns the viewport state creation info
		*/
		vk::PipelineViewportStateCreateInfo make_viewport_state();

		/**
			sets the creation info for the configured rasterizer stage
		*/
		void make_rasterizer_info();
		

		/**
			configures the multisampling stage
		*/
		void configure_multisampling();

		/**
			configures the color blending stage
		*/
		void configure_color_blending();

		/**
			Make a pipeline layout, this consists mostly of describing the
			push constants and descriptor set layouts which will be used.

			\returns the created pipeline layout
		*/
		vk::PipelineLayout make_pipeline_layout();

		/**
			Make a renderpass, a renderpass describes the subpasses involved
			as well as the attachments which will be used.

			\returns the created renderpass
		*/

	};
}