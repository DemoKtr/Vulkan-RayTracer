#pragma once
#include "View/vkMesh/vertexMenagerie.h"
#include "View/vkImage/texture.h"
#include "View/vkInit/vkPipeline/pipelineCache.h"

namespace vkResources {
	extern vkMesh::VertexMenagerie* meshes;

	extern vkImage::Texture* atlasTextures;
	
	extern vkUtil::PipelineCache* scenePipelines;
	extern vkUtil::PipelineCache* editorPipelines;

}