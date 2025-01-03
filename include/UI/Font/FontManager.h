#pragma once
#include "View/vkImage/texture.h"


namespace UI {

	class FontManager {
		std::unordered_map<uint64_t, vkImage::Texture> t;
	};

}