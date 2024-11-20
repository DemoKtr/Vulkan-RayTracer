#pragma once
#include "View/Interface/Thumbs/thumbs.h"
#include "View/vkImage/texture.h"
#include <imgui.h>


namespace vkThumbs {

	class TextureThumbs : public Thumb {

		std::vector<vkImage::Texture*> textures;
	public:
		TextureThumbs(ThumbInput input, fileOperations::filesPaths textures);
		~TextureThumbs();

		ImTextureID get_texture_icon(int index);
	};

}