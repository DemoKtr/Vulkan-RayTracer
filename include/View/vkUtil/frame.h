#pragma once
#include "config.h"
#include <mutex>
#include "View/vkParticle/Particle.h"
#include "descriptorsBuffers.h"
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

	struct alignas(16) FontSBO {
		glm::vec4 PosSize;
		glm::vec4 UVBounds;
		glm::uvec4 textures;
		void printFontSBO() {
			std::cout << "PosSize: ("
				<< PosSize.x << ", "
				<< PosSize.y << ", "
				<< PosSize.z << ", "
				<< PosSize.w << ")\n";

			std::cout << "UVBounds: ("
				<< UVBounds.x << ", "
				<< UVBounds.y << ", "
				<< UVBounds.z << ", "
				<< UVBounds.w << ")\n";

			std::cout << "Textures: ("
				<< textures.x << ", "
				<< textures.y << ", "
				<< textures.z << ", "
				<< textures.w << ")\n";
		}
	};


	class SwapChainFrame {

	public:
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		DescriptorData<vkParticle::Particle>* particleDescriptor;
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
		std::vector<FontSBO> UIFontPositionSize;


		//std::vector<vkParticle::Particle> particlesData;

		glm::vec4 dt;

		//Buffer
		Buffer cameraDataBuffer;
		Buffer modelsDataBuffer;
		//Buffer particleSBODataBuffer;
		Buffer particleUBODataBuffer;
		Buffer UIPositionSizeDataBuffer;
		Buffer UIFontPositionSizeDataBuffer;
		Buffer DeltaTimeDataBuffer;
		//Data Write Location
		void* cameraDataWriteLocation;
		void* modelsDataWriteLocation;
		//void* particleSBODataWriteLocation;
		void* particleUBODataWriteLocation;
		void* DeltaTimeDataWriteLocation;
		void* UIPositionSizeDataWriteLocation;
		void* UIFontPositionSizeDataWriteLocation;
		//DESCRIPTOR BUFFER INFO
		vk::DescriptorBufferInfo cameraUBODescriptor;
		//vk::DescriptorBufferInfo ParticleSBODescriptor;
		vk::DescriptorBufferInfo ParticleUBODescriptor;
		vk::DescriptorBufferInfo DeltaTimeDescriptor;
		vk::DescriptorBufferInfo modelsSBODescriptor;
		vk::DescriptorBufferInfo UIPositionSizeDescriptor;
		vk::DescriptorBufferInfo UIFontPositionSizeDescriptor;
		//DESCRIPTOR SET
		vk::DescriptorSet postprocessDescriptorSet;
		vk::DescriptorSet particleSBODescriptorSet;
		vk::DescriptorSet UIDescriptorSet;
		vk::DescriptorSet UIFontDescriptorSet;


		vk::CommandBuffer mainCommandBuffer;
		vk::CommandBuffer unlitCommandBuffer;
		vk::CommandBuffer pbrCommandBuffer;
		vk::CommandBuffer skyboxCommandBuffer;
		vk::CommandBuffer deferedCommandBuffer;


		glm::mat4 viewProjection;

		//Sync objects
		vk::Semaphore imageAvailable, renderFinished, computeFinished;
		vk::Fence inFlight;

		bool isParticleInit = false;

		void make_depth_resources();
		void make_descriptors_resources(int number_of_objects);
		void write_postprocess_descriptors();
		void write_UI_descriptors();
		void destroy(uint32_t index);

	};
}