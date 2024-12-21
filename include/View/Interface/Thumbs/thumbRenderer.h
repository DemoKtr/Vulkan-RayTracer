#pragma once
#include "config.h"
#include "View/vkMesh/vertexMenagerie.h"
#include "View/vkImage/texture.h"
#include "View/RenderStructs/projection.h"

struct ThumbRendererInput {
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;
	vkMesh::VertexMenagerie* meshes;
	vk::Format pictureFormat;
	vk::Format depthFormat;
	int width;
	int heigh;

};

struct ThumbRendererOutput {
	std::vector<vk::Image> image;
	std::vector<vk::DeviceMemory> imageMemory;
	std::vector<vk::ImageView>  imageView;

};

class ThumbRenderer {
	
	vk::PhysicalDevice physicalDevice;
	vk::Device device;

	vk::RenderPass renderPass;
	vk::Pipeline pipeline;
	vk::PipelineLayout pipelineLayout;
	fileOperations::filesPaths  meshesNames;
	vkImage::Texture* universalTexture;
	
	std::vector<vk::Image> image;
	std::vector<vk::DeviceMemory> imageMemory;
	std::vector<vk::ImageView> imageView;

	std::vector<vk::Image> depthImage;
	std::vector<vk::DeviceMemory> depthImageMemory;
	std::vector<vk::ImageView> depthImageView;

	vk::Sampler sampler;

	vk::Format imageFormat, depthFormat;
	fileOperations::filesPaths models;
	std::vector<vk::Framebuffer> framebuffers;
	//Resource Descriptors
	vk::DescriptorSetLayout descriptorLayout;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorPool descriptorPool;

	vk::Extent2D swapchainExten;

	vk::CommandBuffer commandBuffer;
	vk::Queue queue;

	vkMesh::VertexMenagerie* meshes;

	vkRenderStructs::ViewProjectionData vpData;

	int width, height;
	int number_of_images;
	int number_of_images_buffer;

	void render_objects(int objectType, uint32_t& startInstance);
	void prepare_scene();
	void render_objects(vk::CommandBuffer commandBuffer, int objectType, uint32_t& startInstance, uint32_t instanceCount);
	void debugAccess(
		const std::unordered_map<uint64_t, int>& firstIndices,
		const std::unordered_map<uint64_t, int>& indexCounts,
		uint64_t k);
public:
	
	ThumbRenderer(ThumbRendererInput input, bool debugMode);
	~ThumbRenderer();
	void create_renderpass();
	void create_pipeline();
	void create_framebuffers(bool debug);
	void render(bool debugMode);
	void create_images();
	void create_descriptors_set_layout();
	void make_assets();
	ThumbRendererOutput get_meshes_images();
};
