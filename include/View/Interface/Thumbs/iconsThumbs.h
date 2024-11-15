#pragma once
#include "View/Interface/Thumbs/thumbs.h"
#include <imgui.h>
#include "View/vkImage/texture.h"

namespace vkThumbs {

	class IconThumbs : public Thumb {

		vkImage::Texture* folderIcon;
		vkImage::Texture* headerIcon;
		vkImage::Texture* sourceIcon;
		vkImage::Texture* textIcon;
		vkImage::Texture* fileIcon;
		
	public:
		IconThumbs(ThumbInput input);
		~IconThumbs();
		
		ImTextureID get_folder_icon();
		ImTextureID get_header_icon();
		ImTextureID get_source_icon();
		ImTextureID get_text_icon();
		ImTextureID get_file_icon();

		
	};

}