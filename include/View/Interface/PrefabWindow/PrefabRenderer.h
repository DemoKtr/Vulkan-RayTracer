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
	struct PrefabInput {
		glm::ivec2 screenSize;
		vk::Instance instance;
		//physical device
		vk::PhysicalDevice physicalDevice;
		//logical device
		vk::Device device;
		//queues
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::Queue computeQueue;
		vk::Queue transferQueue;

		vkMesh::VertexMenagerie* meshes;

		vkImage::Texture* atlasTextures;
		
	};
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
		vkMesh::VertexMenagerie* meshes;
		vkImage::Texture* atlasTextures;


		void create_swapchain();
		void create_frame_resources();
		void create_descriptors_set_layouts();
		void create_pipeline();
		void make_assets(Scene* scene);
		void finalize_setup(Scene* scene);
		void recreate_swapchain(Scene* scene);
		void cleanup_swapchain();
	public:
		PrefabRenderer(PrefabInput input, Scene* scene, GLFWwindow* Window);
		void render(Scene* scene,Camera::Camera camera,float deltaTime, bool renderIMGUI);
		void InitImGui(GLFWwindow* window);
	};
}
