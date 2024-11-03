#pragma once
#include "config.h"


namespace vkUtil {


	struct CameraUBO {
		glm::mat4 view;
		glm::vec4 camPos;
	};

	struct MeshSBO {
		glm::mat4 model;
		int textureID;
	};



	class SwapChainFrame {

	public:
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		//Swapchain-type stuff
		vk::Image mainimage;
		vk::ImageView mainimageView;
		//vk::Framebuffer mainframebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;

		/// <summary>
		/// Bufors
		/// </summary>
		/// DATA
		CameraUBO cameraData;
		std::vector<MeshSBO> modelsData;
		//Buffer
		Buffer cameraDataBuffer;
		Buffer modelsDataBuffer;
		//Data Write Location
		void* cameraDataWriteLocation;
		void* modelsDataWriteLocation;
		//DESCRIPTOR BUFFER INFO
		vk::DescriptorBufferInfo cameraUBODescriptor;
		vk::DescriptorBufferInfo modelsSBODescriptor;
		//DESCRIPTOR SET
		vk::DescriptorSet postprocessDescriptorSet;

		int n;


		vk::CommandBuffer commandBuffer;
		vk::CommandBuffer imguiCommandBuffer;
		vk::CommandBuffer postprocessCommandBuffer;
		vk::Framebuffer imguiFrameBuffer;
		vk::Framebuffer postProcessFrameBuffer;

		//Sync objects
		vk::Semaphore imageAvailable, renderFinished, computeFinished;
		vk::Fence inFlight;

		void make_depth_resources();
		void make_descriptors_resources(int number_of_objects);
		void write_postprocess_descriptors();
		void destroy();

	};
}