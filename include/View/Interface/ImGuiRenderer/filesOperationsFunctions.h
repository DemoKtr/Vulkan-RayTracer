#pragma once

#include <string>
#include "fileOperations/filesTypes.h"

namespace vkImGui {



	void render_file_save_window(std::string path, fileOperations::FileType::Type type,bool& isOpen);
	void create_file_function(fileOperations::FileType::Type type, std::string& path, std::string& filename);

}