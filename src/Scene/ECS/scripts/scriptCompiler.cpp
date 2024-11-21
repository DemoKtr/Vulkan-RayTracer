#include "Scene/ECS/scripts/scriptCompiler.h"

bool scripts::compileScript(const std::string& scriptPath, const std::string& outputPath) {
	std::string command = "clang++ -shared -fPIC -o " + outputPath + " " + scriptPath;
	return system(command.c_str()) == 0;
}
