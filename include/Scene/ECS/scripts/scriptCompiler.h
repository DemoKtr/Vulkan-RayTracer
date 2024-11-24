#pragma once
#include <string>
#include "fileOperations/filesFinding.h"
namespace scripts {
    bool compileScript(const std::string& scriptPath, const std::string& outputPath);
    void compileAllScripts(fileOperations::filesPaths& cppFiles, fileOperations::filesPaths& dllFiles);
    void compileAllScripts(fileOperations::filesPaths& cppFiles, fileOperations::filesPaths& dllFiles, int scriptNumber);
}