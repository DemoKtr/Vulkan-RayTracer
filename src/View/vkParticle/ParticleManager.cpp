#include "View/vkParticle/ParticleManager.h"
#include <View/vkInit/vkPipeline/pipelineCache.h>
#include <View/vkResources/resources.h>


vkParticle::ParticleManager::ParticleManager() {
    AddParticleComponent();
    updateThread = std::thread(&ParticleManager::asyncUpdate, this);
}

vkParticle::ParticleManager::~ParticleManager() {
  //  Clear();
    asyncActive = false;

    
}

void vkParticle::ParticleManager::AddParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    particleEmiters.push_back(ecs->getComponent<ParticleComponent>(sceneObject->id));
   
    dirtyFlag = true;
}

void vkParticle::ParticleManager::AddParticleComponent() {
   // particleComponents.push_back(vkParticle::ParticleGenerator());
   // particleEmiters.push_back(new vkParticle::ParticleEmmiter);
}

void vkParticle::ParticleManager::RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    dirtyFlag = true;
}

void vkParticle::ParticleManager::Update(float deltaTime, void* memory, std::vector<vkParticle::Particle>& particles) {
    size_t i = 0;
    if (!dirtyFlag) return;
    for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

        for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
            
            for (vkParticle::Particle& particle : generator.GetParticles()) {
                particles[i++] = particle;

            }
        }
    }
    std::cout << particleEmiters.size() << std::endl;
    memcpy(memory, particles.data(), i * sizeof(vkParticle::Particle));
    dirtyFlag = false;
    particle_to_render = i;
}


void vkParticle::ParticleManager::Render(vkUtil::SwapChainFrame& frame, vk::CommandBuffer& commandBuffer, vk::Extent2D& swapchainExtent, vk::DispatchLoaderDynamic& dldi, vkRenderStructs::PushDataStructure& pushData) {
    // Rozpoczêcie renderowania
    // Pipeline i layout

    
        

    if (particle_to_render <= 0) return;
    vk::RenderingAttachmentInfoKHR colorAttachment = {};
    colorAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
    colorAttachment.imageView = frame.mainimageView; // Widok obrazu.
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;




    vk::RenderingAttachmentInfoKHR depthAttachment = {};
    depthAttachment.sType = vk::StructureType::eRenderingAttachmentInfo;
    depthAttachment.imageView = frame.depthBufferView; // Widok obrazu dla g³êbi.
    depthAttachment.imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eStore;

    // Debugging attachmentów g³êbi


    vk::RenderingInfoKHR renderingInfo = {};
    renderingInfo.sType = vk::StructureType::eRenderingInfoKHR;
    renderingInfo.renderArea.extent.width = swapchainExtent.width;
    renderingInfo.renderArea.extent.height = swapchainExtent.height;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;
    vkUtil::PipelineCacheChunk pipelineInfo = vkResources::scenePipelines->getPipeline("mesh Pipeline");
    try {
        commandBuffer.beginRendering(&renderingInfo);

    }
    catch (vk::SystemError err) {
        std::cerr << "Failed to begin rendering: " << err.what() << std::endl;
        return;
    }

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipeline);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineInfo.pipelineLayout, 0, frame.particleSBODescriptorSet, nullptr);
    vkResources::atlasTextures->useTexture(commandBuffer, pipelineInfo.pipelineLayout);
    commandBuffer.pushConstants(pipelineInfo.pipelineLayout, vk::ShaderStageFlagBits::eTaskEXT, 0, sizeof(vkRenderStructs::PushDataStructure), &pushData);

    commandBuffer.drawMeshTasksEXT((particle_to_render+7)/8, 1, 1, dldi);



    commandBuffer.endRendering();
}

void vkParticle::ParticleManager::updateDescriptorSet() {
    if (isFirstActive.load(std::memory_order_relaxed)) {
        
        std::vector<float> data;
        for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

            for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
                generator.GenerateParticles();
                for (vkParticle::Particle& particle : generator.GetParticles()) {
                    data.push_back(particle.position.x); data.push_back(particle.position.y); data.push_back(particle.position.z);
                    data.push_back(particle.velocity.x); data.push_back(particle.velocity.y); data.push_back(particle.velocity.z);
                    data.push_back(particle.acceleration.x); data.push_back(particle.acceleration.y);// data.push_back(particle.acceleration.z);
                    //data.push_back(particle.size.x); data.push_back(particle.size.y); data.push_back(particle.size.z);
                    //data.push_back(particle.color.x); data.push_back(particle.color.y); data.push_back(particle.color.z);
                   
                }
            }
        }
        
        isFirstActive.store(false, std::memory_order_relaxed);
    }
    else {

        std::vector<float> data;
        for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

            for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
                generator.GenerateParticles();
                for (vkParticle::Particle& particle : generator.GetParticles()) {
                    data.push_back(particle.position.x); data.push_back(particle.position.y); data.push_back(particle.position.z);
                    data.push_back(particle.velocity.x); data.push_back(particle.velocity.y); data.push_back(particle.velocity.z);
                    data.push_back(particle.acceleration.x); data.push_back(particle.acceleration.y);// data.push_back(particle.acceleration.z);
                    //data.push_back(particle.size.x); data.push_back(particle.size.y); data.push_back(particle.size.z);
                    //data.push_back(particle.color.x); data.push_back(particle.color.y); data.push_back(particle.color.z);

                }
            }
        }
        
        isFirstActive.store(true, std::memory_order_relaxed);
    }
}

void vkParticle::ParticleManager::asyncUpdate() {
    while (true) {
       
        this->updateDescriptorSet();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Przyk³adowy interwa³
       
    }
}
