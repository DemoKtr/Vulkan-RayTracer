#pragma once
#include "config.h"
#include "View/vkInit/vkPipeline/pipelineBuilder.h"

namespace vkUtil {

    struct PipelineCacheChunk {
        vk::Pipeline pipeline;
        vk::RenderPass renderPass;
        vk::PipelineLayout pipelineLayout;
        int type = 0;
        // 0 - rendering scene
        // 1 - postprocess
    };

    class PipelineCache {
    private:
        std::unordered_map<std::string, PipelineCacheChunk> pipelines;
        vk::Device device;

    public:
        PipelineCache(vk::Device device);

        vkUtil::PipelineCacheChunk getPipeline(const std::string& name);

        void addPipeline(const std::string& name, const PipelineCacheChunk& pipeline);

        ~PipelineCache();
    };


}