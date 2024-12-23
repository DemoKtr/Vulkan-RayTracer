#pragma once
#include "config.h"
#include <GLFW/glfw3.h>
#include <Scene/scene.h>
#include "Player/camera.h"
#include <View/vkMesh/vertexMenagerie.h>
#include "View/vkImage/texture.h"
#include "View/vkUtil/frame.h"
#include <View/RenderStructs/projection.h>
#include <View/vkMesh/meshesManager.h>


namespace vkPrefab {
	
	class PrefabRenderer {

		glm::ivec2 screenSize;
		int maxFramesInFlight, frameNumber;
		vkRenderStructs::ProjectionData projection;
		bool debugMode{ false };
		GLFWwindow* window;
		vk::Instance instance{ nullptr }; //instancja
		
		vk::PhysicalDevice physicalDevice{ nullptr };
		//logical device
		vk::Device device{ nullptr };
		//queues
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::Queue computeQueue;
		vk::Queue transferQueue;
		//surface
		vk::SurfaceKHR surface;
		//swapchain
		vk::SwapchainKHR swapchain;
		//frames
		std::vector<vkUtil::SwapChainFrame> swapchainFrames;
		//std::vector<vkUtil::SwapChainFrame> swapchainFrames;
		//swapchainFormat
		vk::Format swapchainFormat;
		//swapchainExtent
		vk::Extent2D swapchainExtent;
		//DescriptorPool
		vk::DescriptorPool imguiDescriptorPool;

		vkMesh::MeshesManager* meshesManager;
		

		vk::DescriptorSetLayout MVPDescriptorSetLayout;
		vk::DescriptorSetLayout textureDescriptorSetLayout;


		vk::DescriptorPool MVPDescriptorPool;

		vk::CommandBuffer maincommandBuffer;
		vk::CommandPool imguiCommandPool;
		vk::CommandPool CommandPool;


		void create_swapchain();
		void create_frame_resources(Scene* scene);
		void create_descriptors_set_layouts();
		void create_pipeline();
		void make_assets(Scene* scene);
		void finalize_setup(Scene* scene);
		void recreate_swapchain(Scene* scene);
		void cleanup_swapchain();
		void create_frame_command_buffer();

		void prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera);

		void record_draw_command(vk::CommandBuffer commandBuffer, Scene* scene, uint32_t imageIndex);

		void prepare_scene(vk::CommandBuffer commandBuffer);
		void render_objects(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, uint64_t objectType, uint32_t& startInstance, uint32_t instanceCount);
	public:
		PrefabRenderer(Scene* scene, GLFWwindow* Window);
		void render(Scene* scene,Camera::Camera camera,float deltaTime);
		void InitImGui(GLFWwindow* window);
	};
}
