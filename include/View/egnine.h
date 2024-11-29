#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "config.h"
#include <Scene/scene.h>
#include <Player/camera.h>
#include "View/vkUtil/frame.h"
#include "settings.h"
#include "View/Interface/interface.h"
#include "View/vkMesh/vertexMenagerie.h"
#include <View/vkMesh/meshesManager.h>
#include "View/RenderStructs/projection.h"
#include "fileOperations/filesFinding.h"

class GraphicsEngine {
	glm::ivec2 screenSize;
	const char* appName{ "RayTracer" };

	vkRenderStructs::ProjectionData projection;
	//synchronizers 
	int maxFramesInFlight, frameNumber;
	GLFWwindow* mainWindow;
	bool debugMode{true};
	vk::Instance instance{ nullptr }; //instancja
	//debug callback
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	//dynamic instance dispatcher
	vk::DispatchLoaderDynamic dldi;
	//surface
	vk::SurfaceKHR surface;
	//physical device
	vk::PhysicalDevice physicalDevice{ nullptr };
	//logical device
	vk::Device device{ nullptr };
	//queues
	vk::Queue graphicsQueue{ nullptr };
	vk::Queue presentQueue{ nullptr };
	vk::Queue computeQueue{ nullptr };
	

	//swapchain
	vk::SwapchainKHR swapchain;
	//frames
	std::vector<vkUtil::SwapChainFrame> swapchainFrames;
	//swapchainFormat
	vk::Format swapchainFormat;
	//swapchainExtent
	vk::Extent2D swapchainExtent;

	vk::DescriptorPool imguiDescriptorPool;
	vk::DescriptorPool postprocessDescriptorPool;

	//DESCRIPTOR SET LAYOUT
	vk::DescriptorSetLayout postprocessDescriptorSetLayout;

	vk::RenderPass imguiRenderPass;
	vk::CommandBuffer maincommandBuffer;
	vk::CommandPool imguiCommandPool;
	vk::CommandPool CommandPool;

	vk::DescriptorPool iconDescriptorPool;
	vk::DescriptorSetLayout iconDescriptorSetLayout;

	vk::DescriptorPool textureDescriptorPool;
	vk::DescriptorSetLayout textureDescriptorSetLayout;



	editor* sceneEditor;
	vkMesh::MeshesManager* meshesManager;
	fileOperations::filesPaths  meshesNames;
	fileOperations::filesPaths texturesNames;

	fileOperations::filesPaths cppNames;
	fileOperations::filesPaths dllNames;

	vkMesh::VertexMenagerie* meshes;

	vkImage::Texture* atlasTextures;
	/// <summary>
	///  Pipelines
	/// </summary>
	vk::Pipeline postprocessPipeline;
	vk::PipelineLayout postprocessPipelineLayout;
	vk::RenderPass postprocessRenderPass;
	

	void make_instance(); //instance Setup
	void choice_device();
	void create_swapchain();
	void recreate_swapchain(Scene* scene);
	void cleanup_swapchain();
	void create_descriptor_set_layouts();
	void create_pipeline();
	void finalize_setup(Scene* scene);
	void make_assets(Scene* scene);
	void create_frame_command_buffer();
	void create_frame_resources(Scene* scene);
	void create_framebuffers();

	void load_scripts();

	void record_draw_command(vk::CommandBuffer commandBuffer,Scene* scene,uint32_t imageIndex);

	void prepare_scene(vk::CommandBuffer commandBuffer);
	void render_objects(vk::CommandBuffer commandBuffer, uint64_t objectType, uint32_t& startInstance, uint32_t instanceCount);

	void prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera);
	
public:
	GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode);
	void render(Scene* scene, int& verticesCounter, float deltaTime, Camera::Camera camera, bool renderIMGUI);
	void InitImGui(GLFWwindow* window);
	~GraphicsEngine();
};