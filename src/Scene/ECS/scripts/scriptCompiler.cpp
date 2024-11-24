#include "Scene/ECS/scripts/scriptCompiler.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

bool scripts::compileScript(const std::string& scriptPath, const std::string& outputPath) {

	std::string command;

#ifdef _WIN32
	// Kompilacja dla Windows (64-bit, generowanie DLL)
	command = "clang++ -shared -m64 -o " + outputPath + " " + scriptPath;
#elif defined(__linux__) || defined(__APPLE__)
	// Kompilacja dla Linux/Unix/MacOS (generowanie pliku .so)
	command = "clang++ -shared -fPIC -o " + outputPath + " " + scriptPath;
#else
	std::cerr << "Unsupported platform!" << std::endl;
	return false;
#endif

	// Uruchom komendê systemow¹ i sprawdŸ wynik
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "Compilation failed with error code: " << result << std::endl;
	}
	return result == 0;
}

void scripts::compileAllScripts(fileOperations::filesPaths& cppFiles, fileOperations::filesPaths& dllFiles) {
	size_t i = 0;
	
	for (uint32_t i = 0; i < cppFiles.fullPaths.size(); ++i) {
		std::string dllPath = std::string(PROJECT_DIR) + "\\core\\libs\\" + cppFiles.fileNames[i] + ".dll";
		dllFiles.fullPaths.push_back(dllPath);
		dllFiles.fileNames.push_back(cppFiles.fileNames[i]);
	}

	fileOperations::make_model_index(dllFiles);
	fileOperations::hash_model_paths(dllFiles);

	for (uint32_t i = 0; i < dllFiles.fullPaths.size(); ++i) {
		if (!(compileScript(cppFiles.fullPaths[i], dllFiles.fullPaths[i]))) {
			std::cout << "Script " << dllFiles.fullPaths[i] << " error\n";
		}
	}
}
void scripts::compileAllScripts(fileOperations::filesPaths& cppFiles, fileOperations::filesPaths& dllFiles, int scriptsNumber) {
	size_t i = 0;

	for (uint32_t i = 0; i < scriptsNumber; ++i) {
		std::string dllPath = std::string(PROJECT_DIR) + "\\core\\libs\\" + cppFiles.fileNames[i] + ".dll";
		dllFiles.fullPaths[i] = dllPath;
		dllFiles.fileNames[i] = cppFiles.fileNames[i];
	}

	fileOperations::make_model_index(dllFiles);
	fileOperations::hash_model_paths(dllFiles);

	for (uint32_t i = 0; i < dllFiles.fullPaths.size(); ++i) {
		if (!(compileScript(cppFiles.fullPaths[i], dllFiles.fullPaths[i]))) {
			std::cout << "Script " << dllFiles.fullPaths[i] << " error\n";
		}
	}
}
