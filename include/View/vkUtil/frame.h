#pragma once
#include "config.h"
#include <mutex>

namespace vkUtil {


	struct CameraUBO {
		glm::mat4 view;
		glm::vec4 camPos;
	};

	struct alignas(16) MeshSBO {
		glm::mat4 model;
		uint32_t textureID;
		uint32_t padding[3];
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
		std::vector<glm::vec4> UIPositionSize;
		//Buffer
		Buffer cameraDataBuffer;
		Buffer modelsDataBuffer;
		Buffer UIPositionSizeDataBuffer;
		//Data Write Location
		void* cameraDataWriteLocation;
		void* modelsDataWriteLocation;
		void* UIPositionSizeDataWriteLocation;
		//DESCRIPTOR BUFFER INFO
		vk::DescriptorBufferInfo cameraUBODescriptor;
		vk::DescriptorBufferInfo modelsSBODescriptor;
		vk::DescriptorBufferInfo UIPositionSizeDescriptor;
		//DESCRIPTOR SET
		vk::DescriptorSet postprocessDescriptorSet;
		vk::DescriptorSet UIDescriptorSet;


		vk::CommandBuffer mainCommandBuffer;
		vk::CommandBuffer unlitCommandBuffer;
		vk::CommandBuffer pbrCommandBuffer;
		vk::CommandBuffer skyboxCommandBuffer;
		vk::CommandBuffer deferedCommandBuffer;



		//Sync objects
		vk::Semaphore imageAvailable, renderFinished, computeFinished;
		vk::Fence inFlight;

		

		void make_depth_resources();
		void make_descriptors_resources(int number_of_objects);
		void write_postprocess_descriptors();
		void write_UI_descriptors();
		void destroy();

	};
}