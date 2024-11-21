#pragma once
#include <vector>
#include "Scene/ECS/scripts/scriptLoader.h"
#include "fileOperations/filesFinding.h"

namespace scripts {
	class ScriptManager {
		std::unordered_map<uint64_t, Script> scripts;
		fileOperations::filesPaths files;
		ScriptManager();
		~ScriptManager();
		

	};
}