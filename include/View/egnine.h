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
#include "Scene/Objects/PrefabManager.h"
#include "View/vkImage/cubemap.h"
#include "View/RenderingFlags.h"
#include "UI/UIManagers.h"
#include "UI/Font/FontManager.h"
#include "UI/UIInput.h"
#include "vkParticle/ParticleManager.h"




class GraphicsEngine {
	glm::ivec2 screenSize;
	const char* appName{ "RayTracer" };

	vkRenderStructs::ProjectionData projection;
	//synchronizers 
	int maxFramesInFlight, frameNumber;

	float fullTime = 0.0f;

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
	vk::Queue transferQueue{ nullptr };
	
	uint32_t frameFlag = 1000;
	vkImage::CubemapEctTexture* cubemap;



	vkRenderStructs::PushDataStructure frustum;

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
	vk::DescriptorPool UIDescriptorPool;
	vk::DescriptorPool UIFontSBODescriptorPool;
	vk::DescriptorPool UIFontTextureDescriptorPool;

	//DESCRIPTOR SET LAYOUT
	vk::DescriptorSetLayout postprocessDescriptorSetLayout;
	vk::DescriptorSetLayout UIDescriptorSetLayout;
	vk::DescriptorSetLayout UIFontSBODescriptorSetLayout;
	vk::DescriptorSetLayout UIFontTextureDescriptorSetLayout;


	vk::CommandBuffer maincommandBuffer;
	vk::CommandBuffer transferCommandBuffer;
	vk::CommandBuffer computeCommandBuffer;
	vk::CommandPool CommandPool;
	vk::CommandPool computeCommandPool;
	vk::CommandPool transferCommandPool;

	vk::DescriptorPool iconDescriptorPool;
	vk::DescriptorSetLayout iconDescriptorSetLayout;

	vk::DescriptorPool textureDescriptorPool;

	vk::DescriptorSetLayout textureDescriptorSetLayout;


	vk::DescriptorPool cubemapDescriptorPool;
	vk::DescriptorSetLayout cubemapDescriptorSetLayout;

	

	vk::DescriptorPool particleDescriptorPool;
	vk::DescriptorSetLayout particleDescriptorSetLayout;

	vk::DescriptorPool waterDescriptorPool;
	vk::DescriptorSetLayout waterDescriptorSetLayout;

	vk::DescriptorPool fogDescriptorPool;
	vk::DescriptorSetLayout fogDescriptorSetLayout;


	vk::DescriptorPool fogDataDescriptorPool;
	vk::DescriptorSetLayout fogDataDescriptorSetLayout;

	postprocess::Fog *fog;

	editor* sceneEditor;
	vkMesh::MeshesManager* meshesManager;
	UI::UIManager UImanager;
	
	// TO DO DELETE THIS:
	UI::UIButton* test;
	UI::UIText* testText;
	UI::FontManager* fontManager;

	UI::UIRenderingDrawData uiRenderingDrawData;

	vkPrefab::PrefabManager& prefabsManager = vkPrefab::PrefabManager::getInstance();
	RenderObjects objects_to_rendering;

	//math::FastFourierTransform *fft;

	vkParticle::ParticleManager* particleManager;

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
	void create_frame_resources(int number_of_models);

	
	void load_meshes_files();
	void load_textures_files();
	void load_scripts();
	void load_fonts();

	void record_draw_command(vk::CommandBuffer commandBuffer, vk::CommandBuffer unlitCommandBuffer,Scene* scene,uint32_t imageIndex, float dt);
	void record_unlit_draw_command(vk::CommandBuffer commandBuffer,uint32_t imageIndex);
	void record_pbr_draw_command(vk::CommandBuffer commandBuffer,uint32_t imageIndex);

	void prepare_scene(vk::CommandBuffer commandBuffer);
	void render_objects(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, uint64_t objectType, uint32_t& startInstance, uint32_t instanceCount);

	void prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera);

	void build_accelerationStructures();
	
	void initial_cubemap();
public:
	GraphicsEngine(glm::ivec2 screenSize, GLFWwindow* window, Scene* scene, bool debugMode);
	void render(Scene* scene, int& verticesCounter, float deltaTime, Camera::Camera camera, bool renderIMGUI);
	void InitImGui(GLFWwindow* window);
	~GraphicsEngine();
};