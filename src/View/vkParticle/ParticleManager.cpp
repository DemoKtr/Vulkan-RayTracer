#include "View/vkParticle/ParticleManager.h"
#include <View/vkInit/vkPipeline/pipelineCache.h>
#include <View/vkResources/resources.h>
#include <MultithreatedSystems/mutexManager.h>


vkParticle::ParticleManager::ParticleManager(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Queue queue, vk::CommandBuffer commandBuffer) {
    AddParticleComponent();
    updateThread = std::thread(&ParticleManager::asyncUpdate, this);
    vkUtil::DescriptorData<vkParticle::ParticleInit>* pDesc = new vkUtil::DescriptorData<vkParticle::ParticleInit>(physicalDevice, device);
    vkUtil::DescriptorData<vkParticle::ParticleInit>* pDesc2 = new vkUtil::DescriptorData<vkParticle::ParticleInit>(physicalDevice, device);
    vkUtil::DescriptorDataInput input;
    input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
    input.canTransfer = true;
    input.count = maxParticleCount;

    dirtyFlag = new bool(true);
    auto& mutexManager = MutexManager::getInstance();
    mutexManager.addMutex("particle");
    pDesc->make_descriptors_resources(input);
    pDesc2->make_descriptors_resources(input);
    firstParticleRandomDescriptor = pDesc;
    seccondParticleRandomDescriptor = pDesc2;

    this->physicalDevice = physicalDevice;
    this->logicalDevice = device;
    this->queue = queue;
    this->commandBuffer = commandBuffer;
}

vkParticle::ParticleManager::~ParticleManager() {
  //  Clear();
    
    asyncActive = false;
    
    delete firstParticleRandomDescriptor;
  
    delete seccondParticleRandomDescriptor;
    
    updateThread.join();
    delete dirtyFlag;
}

void vkParticle::ParticleManager::AddParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    std::shared_ptr<ParticleComponent> comp = ecs->getComponent<ParticleComponent>(sceneObject->id);
    comp.get()->write_flag_pointer(dirtyFlag);
    particleEmiters.push_back(comp);
   
    *dirtyFlag = true;
}

void vkParticle::ParticleManager::AddParticleComponent() {
   // particleComponents.push_back(vkParticle::ParticleGenerator());
   // particleEmiters.push_back(new vkParticle::ParticleEmmiter);
}

void vkParticle::ParticleManager::RemoveParticleComponent(ecs::ECS* ecs, SceneObject* sceneObject) {
    *dirtyFlag = true;
}

void vkParticle::ParticleManager::Update(float deltaTime, void* memory, std::vector<vkParticle::Particle>& particles) {
    size_t i = 0;
    if (!*dirtyFlag) return;
    for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

        for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
            generator.GenerateParticles();
            for (vkParticle::Particle& particle : generator.GetParticles()) {
                particles[i++] = particle;

            }
        }
    }
    std::cout << particleEmiters.size() << std::endl;
    memcpy(memory, particles.data(), i * sizeof(vkParticle::Particle));
    *dirtyFlag = false;
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

vkUtil::DescriptorData<vkParticle::ParticleInit>* vkParticle::ParticleManager::getActiveDescriptor() {
    if (isFirstActive.load(std::memory_order_relaxed))
        return firstParticleRandomDescriptor;
    else
        return seccondParticleRandomDescriptor;
}

vkUtil::DescriptorData<vkParticle::ParticleInit>* vkParticle::ParticleManager::getFirstDescriptor()
{
    return firstParticleRandomDescriptor;
}

vkUtil::DescriptorData<vkParticle::ParticleInit>* vkParticle::ParticleManager::getSeccondDescriptor()
{
    return seccondParticleRandomDescriptor;
}

void vkParticle::ParticleManager::setSwapchainPointers(vkUtil::SwapChainFrame& frame) {
    frame.firstParticleRandomSSBO = this->firstParticleRandomDescriptor;
    frame.seccondParticleRandomSSBO = this->seccondParticleRandomDescriptor;
    frame.isFirstSSBOParticleActive = &this->isFirstActive;
}

void vkParticle::ParticleManager::updateDescriptorSet() {
    if (isFirstActive.load(std::memory_order_relaxed)) {
        
        std::vector<ParticleInit> data;
        for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

            for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
                generator.GenerateParticles(data);
                for (vkParticle::Particle& particle : generator.GetParticles()) {
                    
                    data.push_back(ParticleInit(particle));

                }
            }
        }
        if (data.size() <= 0)return;
        BufferInputChunk inputChunk;
        inputChunk.logicalDevice = logicalDevice;
        inputChunk.physicalDevice = physicalDevice;
        inputChunk.size = sizeof(ParticleInit) * data.size();
        inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
        inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
            | vk::MemoryPropertyFlagBits::eHostCoherent;
        Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);
        inputChunk.memoryAllocatet = vk::MemoryAllocateFlagBits::eDeviceAddress;
        //fill it with vertex data
        void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
        memcpy(memoryLocation, data.data(), inputChunk.size);

        logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

        //copy to it
        vkUtil::copyBuffer(
            stagingBuffer, firstParticleRandomDescriptor->dataBuffer, inputChunk.size,
            queue, commandBuffer
        );

        //destroy staging buffer
        logicalDevice.destroyBuffer(stagingBuffer.buffer);
        logicalDevice.freeMemory(stagingBuffer.bufferMemory);


        isFirstActive.store(false, std::memory_order_relaxed);
    }
    else {

        std::vector<ParticleInit> data;
        for (std::shared_ptr<ParticleComponent> component : particleEmiters) {

            for (vkParticle::ParticleGenerator& generator : component.get()->getEmiter()->getGenerators()) {
                generator.GenerateParticles();
                for (vkParticle::Particle& particle : generator.GetParticles()) {
                    
                    data.push_back(ParticleInit(particle));

                }
            }
        }
        if (data.size() <= 0)return;
        BufferInputChunk inputChunk;
        inputChunk.logicalDevice = logicalDevice;
        inputChunk.physicalDevice = physicalDevice;
        inputChunk.size = sizeof(ParticleInit) * data.size();
        inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
        inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
            | vk::MemoryPropertyFlagBits::eHostCoherent;
        Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);
        inputChunk.memoryAllocatet = vk::MemoryAllocateFlagBits::eDeviceAddress;
        //fill it with vertex data
        void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
        memcpy(memoryLocation, data.data(), inputChunk.size);

        logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

        //copy to it
        vkUtil::copyBuffer(
            stagingBuffer, firstParticleRandomDescriptor->dataBuffer, inputChunk.size,
            queue, commandBuffer
        );

        //destroy staging buffer
        logicalDevice.destroyBuffer(stagingBuffer.buffer);
        logicalDevice.freeMemory(stagingBuffer.bufferMemory);


        isFirstActive.store(true, std::memory_order_relaxed);
    }
}

void vkParticle::ParticleManager::asyncUpdate() {
    while (asyncActive) {
       
        this->updateDescriptorSet();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Przyk³adowy interwa³
       
    }
}
