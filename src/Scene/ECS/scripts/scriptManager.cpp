#include "Scene/ECS/scripts/scriptManager.h"
#include "fileOperations/filesFinding.h"

scripts::ScriptManager::ScriptManager() {
	std::vector<std::string> ext = { ".dll" };

	list_files_in_directory("\\core", files, ext);

	

	for (uint32_t i = 0; i < this->files.fullPaths.size(); ++i) {
		Script script(this->files.fullPaths[i]);
		this->scripts[this->files.hash[this->files.fullPaths[i]]] = script;
	}
}

scripts::ScriptManager::~ScriptManager() {

}



