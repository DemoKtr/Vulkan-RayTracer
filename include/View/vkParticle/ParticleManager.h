#pragma once
#include "config.h"
#include <Scene/sceneObject.h>
#include <Scene/ECS/components/components.h>
#include "View/RenderStructs/frustum.h"
#include "View/vkUtil/frame.h"
#include "View/vkImage/textureStorage.h"
#include <thread>
#include "View/vkUtil/descriptorsBuffers.h"
namespace vkParticle {
    class ParticleManager {
        bool* dirtyFlag;
       
       
        size_t maxParticleCount = 10000000;
        std::atomic<bool> isFirstActive{ true };
        bool asyncActive = true;
        std::thread updateThread;
        vkUtil::DescriptorData<vkParticle::ParticleInit>* firstParticleRandomDescriptor;
        vkUtil::DescriptorData<vkParticle::ParticleInit>* seccondParticleRandomDescriptor;


        vk::PhysicalDevice physicalDevice;
        vk::Device logicalDevice;
        vk::Queue queue;
        vk::CommandBuffer commandBuffer;
    public:
        size_t particle_to_render = 0;
        // Konstruktor
        ParticleManager(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::CommandBuffer commandBuffer);
        ~ParticleManager();

        // Dodaj komponent cz¹steczek do mened¿era
        void AddParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject);
        void AddParticleComponent();
    

        // Usuñ komponent cz¹steczek z mened¿era
        void RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject);

        // Aktualizuj wszystkie komponenty cz¹steczek
        void Update(float deltaTime, void* memory, std::vector<vkParticle::Particle>& particles);

        // Renderuj wszystkie komponenty cz¹steczek
        void Render(vkUtil::SwapChainFrame& frame, vk::CommandBuffer& commandBuffer, vk::Extent2D& swapchainExtent, vk::DispatchLoaderDynamic& dldi, vkRenderStructs::PushDataStructure& pushData);

        // Usuñ wszystkie komponenty
        void Clear();// {
            //for (std::shared_ptr<ParticleComponent> emiter : particleEmiters) {
        vkUtil::DescriptorData<vkParticle::ParticleInit>* getActiveDescriptor();
        vkUtil::DescriptorData<vkParticle::ParticleInit>* getFirstDescriptor();
        vkUtil::DescriptorData<vkParticle::ParticleInit>* getSeccondDescriptor();

        void setSwapchainPointers(vkUtil::SwapChainFrame& frame);
            //}
       // }

    private:
        std::vector<std::shared_ptr<ParticleComponent>> particleEmiters;
        void updateDescriptorSet();
        void asyncUpdate();
    };
}